#include "SqlUserData.hpp"


SqlUserData::SqlUserData(SqlOps* ops)
{
    m_sql = std::shared_ptr<SqlOps>(ops);
}

SqlUserData::~SqlUserData()
{
}

bool SqlUserData::handleInsert(const std::string &mail, const std::string &pwd)
{
    std::string query = "insert into user (id, email, password) values (?, ? , ?);";

    m_sql->initSql(m_sql->getStmt(), query);
    m_sql->set_param_null(0);
    m_sql->set_param_bind(1, mail);
    m_sql->set_param_bind(2, pwd);
    return m_sql->pre_excute();
}

bool SqlUserData::handleUpdate(const std::string &pwd, const std::string &mail)
{
    std::string query = "update user set password=? where email=?;";

    m_sql->initSql(m_sql->getStmt(), query);
    m_sql->set_param_bind(0, pwd);
    m_sql->set_param_bind(1, mail);
    return m_sql->pre_excute();
}

std::string SqlUserData::handleQuery(const std::string &mail)
{
    std::string query = "select password from user where email=?;";

    m_sql->initSql(m_sql->getStmt(), query);
    m_sql->set_param_bind(0, mail);
    m_sql->query();
    unsigned long len = 20;
    char* buffer[20];
    m_sql->set_param_result(0, MYSQL_TYPE_STRING, *buffer, sizeof(buffer), &len );
    m_sql->get_result();
    m_sql->fetch_result();
    query = *buffer;
    return query;
}

bool SqlUserData::handleDel(const std::string &mail)
{
    std::string query = "delete from user where email= ?;";

    m_sql->initSql(m_sql->getStmt(), query);
    m_sql->set_param_bind(0, mail);
    return m_sql->pre_excute();
}
