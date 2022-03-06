#ifndef SQLCONNPOOL_H_
#define SQLCONNPOOL_H_

#include <string>
#include <queue>
#include <mutex>
#include <memory>
#include "SqlOps.hpp"
#include <mysql/mysql.h>
#include <Singleton.hpp>

class SqlConnPool {
    public:
        SqlConnPool();
        explicit SqlConnPool(SqlConnPool*);
        ~SqlConnPool();

        // 初始化数据库连接的相关数据
        bool Init(const std::string& host, int port, const std::string& user,
                  const std::string& pwd, const std::string& db_name,
                  unsigned long conn_name);

        // 关闭连接池 
        void ClosePool();

        // 获取连接池中的对象
        std::unique_ptr<SqlOps> getConnObj();
        // 释放连接对象
        void FreeConnObj(std::unique_ptr<SqlOps> conn);

    private:
        void DestoryConnection(std::unique_ptr<SqlOps> conn);


    private:
        std::queue<std::unique_ptr<SqlOps>> m_connQue;
        int m_max_conn;
        std::mutex m_mutex;
};

using g_sqlMgr = Singleton<SqlConnPool>;

#endif
