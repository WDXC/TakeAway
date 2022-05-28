#include "SqlHandler.hpp"

SqlHandler::SqlHandler(SqlConnPool* connpool)
{
    if (connpool) {
        m_sql = connpool->getConnObj();
        m_connpool = connpool;
        m_user = new SqlUserData(m_sql);
        m_merchant = new SqlMerchant(m_sql);
    }
}

SqlHandler::SqlHandler(SqlOps **sql, SqlConnPool *connpool)
{
    if (connpool) {
        *sql = connpool->getConnObj();
        m_sql = *sql;
        m_user = new SqlUserData(m_sql);
        m_merchant = new SqlMerchant(m_sql);
        m_connpool = connpool;
    }
}

SqlHandler::SqlHandler(SqlHandler *obj)
{
    m_sql = obj->m_sql;
    m_user = obj->m_user;
    m_connpool = obj->m_connpool;
}

SqlHandler::~SqlHandler() {
    if (m_sql) {
        m_connpool->FreeConnObj(m_sql);
    }
}

bool SqlHandler::userInsert(const std::string &mail, const std::string &pwd, const std::string& username)
{
    return m_user->handleInsert(mail, pwd, username);
}


bool SqlHandler::userUpdate(const std::string& mail, const std::string &pwd) {
    return m_user->handleUpdate(mail, pwd);
}

std::string SqlHandler::userQuery(const std::string &email)
{
    return m_user->handleQuery(email);
}

bool SqlHandler::userDel(const std::string mail)
{
    return m_user->handleDel(mail);
}

bool SqlHandler::merchantInsert(const std::string &username, const std::string &mail, const std::string &pwd)
{
    return m_merchant->handleInsert(username, mail, pwd);
}

bool SqlHandler::merchantUpdate(const std::string &mail, const std::string &pwd)
{
    return m_merchant->handleUpdate(mail, pwd);
}

std::string SqlHandler::merchantQuery(const std::string &mail)
{
    return m_merchant->handleQuery(mail);
}

bool SqlHandler::merchantDel(const std::string &mail)
{
    return m_merchant->handleDel(mail);
}
