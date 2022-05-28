#ifndef SQLUSERDATA_H_
#define SQLUSERDATA_H_

#include "SqlOps.hpp"
#include "SqlConnPool.hpp"
#include <Log.hpp>
#include <memory>

class SqlUserData {
public:
    SqlUserData(SqlOps* ops);
    ~SqlUserData();

    bool handleInsert(const std::string& mail, const std::string& pwd, const std::string &username);
    bool handleUpdate(const std::string& pwd, const std::string& mail);
    std::string handleQuery(const std::string& mail);
    bool handleDel(const std::string& mail);

private:
    std::shared_ptr<SqlOps> m_sql;
};


#endif
