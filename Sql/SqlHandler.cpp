#include "SqlHandler.hpp"

SqlHandler::SqlHandler(std::unique_ptr<SqlConnPool> connpool) :
    m_sql(std::make_unique<SqlOps>()){
    if (connpool) {
        m_sql = connpool->getConnObj();
        m_connpool = std::move(connpool);
        m_userdata = std::make_unique<SqlUserData>(std::move(m_sql));
    }
}

SqlHandler::SqlHandler(std::unique_ptr<SqlOps> sql, std::unique_ptr<SqlConnPool> connpool) :
    m_sql(std::make_unique<SqlOps>())
{
    if (connpool) {
        sql = connpool->getConnObj();
        m_sql = std::move(sql);
        m_userdata = std::make_unique<SqlUserData>(std::move(m_sql));
        m_connpool = std::move(connpool);
    }
}

SqlHandler::~SqlHandler() {
    if (m_sql) {
        m_connpool->FreeConnObj(std::move(m_sql));
    }
}

bool SqlHandler::dealInsert()
{
    return m_userdata->handleInsert("aa", "bb");
}


bool SqlHandler::dealUpdate() {
    return m_userdata->handleUpdate("root123", "2342");
}

std::string SqlHandler::dealQuery(const std::string &pwd)
{
    return m_userdata->handleQuery(pwd);
}

bool SqlHandler::dealDel()
{
    return m_userdata->handleDel("44r");
}
