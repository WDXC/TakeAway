#ifndef SQLHANDLER_H
#define SQLHANDLER_H

#include <memory>
#include "SqlConnPool.hpp"
#include "SqlUserData.hpp"

class SqlHandler {
public:
    explicit SqlHandler(SqlConnPool* connpool);
    SqlHandler(SqlOps** sql, SqlConnPool* connpool);
    SqlHandler(SqlHandler* obj);
    ~SqlHandler();

    // user table operator
    bool userInsert(const std::string& mail, const std::string& pwd);
    bool userUpdate(const std::string& mail, const std::string& pwd);
    std::string userQuery(const std::string& pwd);
    bool userDel(const std::string mail);

    // admin table operator

private:
    SqlOps* m_sql;
    SqlConnPool* m_connpool;
    SqlUserData* m_userdata;
};

#endif //NETLIB_SQLHANDLER_H
