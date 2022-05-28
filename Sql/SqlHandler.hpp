#ifndef SQLHANDLER_H
#define SQLHANDLER_H

#include <memory>
#include "SqlConnPool.hpp"
#include "SqlUserData.hpp"
#include "SqlMerchant.hpp"

class SqlHandler {
public:
    explicit SqlHandler(SqlConnPool* connpool);
    SqlHandler(SqlOps** sql, SqlConnPool* connpool);
    SqlHandler(SqlHandler* obj);
    ~SqlHandler();

    // user login/signup table operator
    bool userInsert(const std::string& mail, const std::string& pwd, const std::string &username);
    bool userUpdate(const std::string& mail, const std::string& pwd);
    std::string userQuery(const std::string& mail);
    bool userDel(const std::string mail);

    // merchant login/signup table operator
    bool merchantInsert(const std::string& username, const std::string& mail, const std::string& pwd);
    bool merchantUpdate(const std::string& mail, const std::string& pwd);
    std::string merchantQuery(const std::string& mail);
    bool merchantDel(const std::string& mail);

private:
    SqlOps* m_sql;
    SqlConnPool* m_connpool;
    SqlUserData* m_user;
    SqlMerchant* m_merchant;
};

#endif //NETLIB_SQLHANDLER_H
