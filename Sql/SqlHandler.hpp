#ifndef SQLHANDLER_H
#define SQLHANDLER_H

#include <memory>
#include "SqlConnPool.hpp"
#include "SqlUserData.hpp"

class SqlHandler {
public:
    explicit SqlHandler(std::unique_ptr<SqlConnPool> connpool);
    SqlHandler(std::unique_ptr<SqlOps> sql, std::unique_ptr<SqlConnPool> connpool);
    ~SqlHandler();

    bool dealInsert();
    bool dealUpdate();
    std::string dealQuery(const std::string& pwd);
    bool dealDel();

private:
    std::unique_ptr<SqlOps> m_sql;
    std::unique_ptr<SqlConnPool> m_connpool;
    std::unique_ptr<SqlUserData> m_userdata;
};

#endif //NETLIB_SQLHANDLER_H
