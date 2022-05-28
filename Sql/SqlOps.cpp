#include "SqlOps.hpp"
#include <Log.hpp>

SqlOps::SqlOps() :
    isConnected(false),
    bCommit(false),
    reconnected(false),
    sql_result(NULL),
    sql_stmt(NULL),
    m_param_bind(NULL),
    m_result_bind(NULL),
    sql_stmt_result(NULL),
    m_param_count(0),
    m_result_count(0) {
    mysql_init(&sql_conn);
    sql_stmt = mysql_stmt_init(&sql_conn);
}

SqlOps::SqlOps(SqlOps *)
{

}

SqlOps::~SqlOps()
{
    free();
    FreeResult();
    CloseConnect();
}

bool SqlOps::connect(const char *ip, int port, const char *username, const char *password, const char *db_name)
{
    if (isConnected && !reconnected) {
        return true;
    }

    hostIp_ = ip;
    port_ = port;
    username_ = username;
    password_ = password;
    db_name_ = db_name;

    if (mysql_real_connect(&sql_conn, ip, username, password,
                           db_name, port, NULL, 0) == NULL) {
        LOG_ERROR("connect failed %s \n", mysql_error(&sql_conn));
        return false;
    }
    isConnected = true;
    LOG_INFO("connect establish [user:%s]\n", username);
    return true;
}

void SqlOps::CloseConnect()
{
    mysql_close(&sql_conn);
    isConnected = false;
}

bool SqlOps::ExecuteQuery(const char *sql)
{
    if (sql == NULL) {
        return true;
    }
    if (!isConnected) {
        LOG_ERROR("connection don't established!\n");
        bCommit = false;
    }
    try {
        if (mysql_real_query(&sql_conn, sql, strlen(sql)) != 0) {
            LOG_ERROR("select query error\n");
            bCommit = false;
            return false;
        }
        FreeResult();
        sql_result = mysql_store_result(&sql_conn);
    }  catch (...) {
        ReConnect();
        isConnected = true;
        bCommit = false;
        return false;
    }
    fieldcnt = mysql_num_fields(sql_result);
    mapFieldToIndex.clear();
    MYSQL_FIELD *fields = mysql_fetch_field(sql_result);
    for (int i = 0; i < fieldcnt; ++i) {
        mapFieldToIndex[fields[i].name] = i;
    }
    return true;
}

bool SqlOps::Execute(const char *sql)
{
    if (sql == NULL) {
        return true;
    }
    if (!isConnected) {
        LOG_ERROR("connection don't established!\n");
        bCommit = false;
    }
    try {
        if (mysql_real_query(&sql_conn, sql, strlen(sql)) != 0) {
            LOG_ERROR("select query error\n");
            bCommit = false;
            return false;
        }
        FreeResult();
        sql_result = mysql_store_result(&sql_conn);
    }  catch (...) {
        ReConnect();
        isConnected = true;
        bCommit = false;
        return false;
    }
    return true;
}

bool SqlOps::startTransaction()
{
    // 开启事务
    bCommit = true;
    return Execute("START TRANSACTION;");
}

bool SqlOps::EndTransaction()
{
    if (bCommit) {
        // 提交事务
        Execute("COMMIT;");
        return true;
    } else {
        // 事务回滚
        Execute("ROLLBACK;");
        return false;
    }
}

// 获取行数
int SqlOps::GetQueryResultCount()
{
    return mysql_num_rows(sql_result);
}

char *SqlOps::GetField(const char *fieldname)
{
    return GetField(mapFieldToIndex[fieldname]);
}

char *SqlOps::GetField(int fieldnum)
{
    if (fieldnum >= fieldcnt) {
        LOG_ERROR("the index is out of bound");
        return NULL;
    }
    return sql_row[fieldnum];
}

// 获取结果中的一行
char **SqlOps::FetchRow()
{
    if (sql_result == NULL) {
        return NULL;
    }
    sql_row = mysql_fetch_row(sql_result);
    return sql_row;
}

MYSQL &SqlOps::getConn()
{
    return sql_conn;
}

void SqlOps::EscapeString(char *dst, char *src)
{
    mysql_real_escape_string(&sql_conn, dst, src, strlen(src));
}



MYSQL_STMT &SqlOps::getStmt()
{
    return *sql_stmt;
}

bool SqlOps::initSql(MYSQL_STMT &stmt, std::string &sql)
{
    if (mysql_stmt_prepare(&stmt, sql.c_str(), sql.size())) {
        return false;
    }
    m_param_count = mysql_stmt_param_count(&stmt);
    if (m_param_count > 0) {
        m_param_bind = new MYSQL_BIND[m_param_count];
        if (!m_param_bind) {
            return false;
        }
        memset(m_param_bind, 0, sizeof(MYSQL_BIND)*m_param_count);
    }
    return true;
}

bool SqlOps::set_param_bind(int index, int &value)
{
    if (index >= m_param_count) {
        LOG_ERROR("Out of the index!");
        return false;
    }
    m_param_bind[index].buffer_type = MYSQL_TYPE_LONG;
    m_param_bind[index].buffer = &value;
    return true;
}

bool SqlOps::set_param_bind(int index, const std::string &value)
{
    if (index >= m_param_count) {
        LOG_ERROR("Out of the index!");
        return false;
    }
    m_param_bind[index].buffer_type = MYSQL_TYPE_STRING;
    m_param_bind[index].buffer = (void*)(value.c_str());
    m_param_bind[index].buffer_length = value.size();
    return true;
}

bool SqlOps::set_param_null(int index)
{
    if (index >= m_param_count) {
        LOG_ERROR("out of the index!");
        return false;
    }
    m_param_bind[index].buffer_type = MYSQL_TYPE_NULL;
    return true;
}

bool SqlOps::set_param_result(int index, enum_field_types type, char *buffer, unsigned long buffer_len, unsigned long *len)
{
    if (index >= m_param_count) {
        LOG_ERROR("Out of the index!");
        return false;
    }
    m_result_bind[index].buffer_type = type;
    m_result_bind[index].buffer = buffer;
    m_result_bind[index].buffer_length = buffer_len;
    m_result_bind[index].length = len;
    return true;
}

bool SqlOps::pre_excute()
{
    if (!sql_stmt) {
        return false;
    }
    if (mysql_stmt_bind_param(sql_stmt, m_param_bind)) {
        return false;
    }
    if (mysql_stmt_execute(sql_stmt)) {
        return false;
    }

    if (0 == mysql_stmt_affected_rows(sql_stmt)) {
        LOG_ERROR("update failed!");
        return false;
    }
    return true;
}

bool SqlOps::query()
{
    if (!sql_stmt) {
        return false;
    }
    if (mysql_stmt_bind_param(sql_stmt, m_param_bind)) {
        return false;
    }
    sql_stmt_result = mysql_stmt_result_metadata(sql_stmt);
    if (sql_stmt_result == NULL) {
        return false;
    } else {
        m_result_count = mysql_num_fields(sql_stmt_result);
        m_result_bind = new MYSQL_BIND[m_result_count];
        if (!m_result_bind) {
            return false;
        }
        memset(m_result_bind, 0, sizeof (MYSQL_BIND)*m_result_count);
    }
    if (mysql_stmt_execute(sql_stmt)) {
        return false;
    }
    return true;
}

bool SqlOps::get_result()
{
    if (mysql_stmt_bind_result(sql_stmt, m_result_bind)) {
        return false;
    }
    if (mysql_stmt_store_result(sql_stmt)) {
        return false;
    }
    return true;
}


bool SqlOps::fetch_result()
{
    if (!mysql_stmt_fetch(sql_stmt))
    {
        return false;
    }
    else
    {
        return true;
    }
}


void SqlOps::free()
{
    if (sql_stmt) {
        mysql_stmt_close(sql_stmt);
        sql_stmt = NULL;
    }
    if (m_param_bind) {
        delete [] m_param_bind;
        m_param_bind = NULL;
    }
    if (m_result_bind) {
        delete [] m_result_bind;
        m_result_bind = NULL;
    }
    if (NULL != sql_stmt_result) {
        mysql_free_result(sql_stmt_result);
        sql_stmt_result = NULL;
    }
    m_param_bind = 0;
    m_result_bind = 0;
}


bool SqlOps::ReConnect()
{
    reconnected = true;
    CloseConnect();
    return connect(hostIp_.c_str(), port_, username_.c_str(), password_.c_str(), db_name_.c_str());
}

void SqlOps::FreeResult()
{
    if (sql_result != NULL) {
        mysql_free_result(sql_result);
        sql_result = NULL;
    }
}

