#include "SqlUserData.hpp"


SqlUserData::SqlUserData(SqlOps* ops)
{
    m_sql = std::shared_ptr<SqlOps>(ops);
}

SqlUserData::~SqlUserData()
{
}

bool SqlUserData::handleInsert(const std::string &mail, const std::string &pwd, const std::string& username)
{
    std::string query = "insert into sf_user (id, UserName, Email, UserPassword) values (?, ?, ? , ?);";

    m_sql->initSql(m_sql->getStmt(), query);
    m_sql->set_param_null(0);
    m_sql->set_param_bind(1, username);
    m_sql->set_param_bind(2, mail);
    m_sql->set_param_bind(3, pwd);
    return m_sql->pre_excute();
}

bool SqlUserData::handleUpdate(const std::string &pwd, const std::string &mail)
{
    std::string query = "update sf_user set UserPassword=? where Email=?;";

    m_sql->initSql(m_sql->getStmt(), query);
    m_sql->set_param_bind(0, pwd);
    m_sql->set_param_bind(1, mail);
    return m_sql->pre_excute();
}

std::string SqlUserData::handleQuery(const std::string &mail)
{
    std::string query = "select UserPassword from sf_user where Email=?;";

    m_sql->initSql(m_sql->getStmt(), query);
    m_sql->set_param_bind(0, mail);
    m_sql->query();
    unsigned long len = 128;
    char* buffer[32];
    m_sql->set_param_result(0, MYSQL_TYPE_STRING, *buffer, sizeof(buffer), &len );
    m_sql->get_result();
    m_sql->fetch_result();
    query = *buffer;
    return query;
}

bool SqlUserData::handleDel(const std::string &mail)
{
    std::string query = "delete from sf_user where Email= ?;";

    m_sql->initSql(m_sql->getStmt(), query);
    m_sql->set_param_bind(0, mail);
    return m_sql->pre_excute();
}
