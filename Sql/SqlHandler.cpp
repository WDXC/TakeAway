#include "SqlHandler.hpp"

SqlHandler::SqlHandler(SqlConnPool* connpool)
{
    if (connpool) {
        m_sql = connpool->getConnObj();
        m_connpool = connpool;
        m_userdata = new SqlUserData(m_sql);
    }
}

SqlHandler::SqlHandler(SqlOps **sql, SqlConnPool *connpool)
{
    if (connpool) {
        *sql = connpool->getConnObj();
        m_sql = *sql;
        m_userdata = new SqlUserData(m_sql);
        m_connpool = connpool;
    }
}

SqlHandler::SqlHandler(SqlHandler *obj)
{
    m_sql = obj->m_sql;
    m_userdata = obj->m_userdata;
    m_connpool = obj->m_connpool;
}

SqlHandler::~SqlHandler() {
    if (m_sql) {
        m_connpool->FreeConnObj(m_sql);
    }
}

bool SqlHandler::userInsert(const std::string &mail, const std::string &pwd)
{
    return m_userdata->handleInsert(mail, pwd);
}


bool SqlHandler::userUpdate(const std::string& mail, const std::string &pwd) {
    return m_userdata->handleUpdate(mail, pwd);
}

std::string SqlHandler::userQuery(const std::string &email)
{
    return m_userdata->handleQuery(email);
}

bool SqlHandler::userDel(const std::string mail)
{
    return m_userdata->handleDel(mail);
}
