#ifndef SQLMERCHANT_H_
#define SQLMERCHANT_H_

#include "SqlOps.hpp"
#include "SqlConnPool.hpp"
#include <Log.hpp>
#include <memory>

class SqlMerchant{
public:
    SqlMerchant(SqlOps* ops);
    ~SqlMerchant();

    bool handleInsert(const std::string &username, const std::string& mail, const std::string& pwd);
    bool handleUpdate(const std::string& mail, const std::string& pwd);
    int queryMail(const std::string mail);
    std::string handleQuery(const std::string& mail);
    bool handleDel(const std::string& mail);

private:
    std::shared_ptr<SqlOps> m_sql;
};


#endif
