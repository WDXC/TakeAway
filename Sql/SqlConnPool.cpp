#include "SqlConnPool.hpp"
#include "Log.hpp"

SqlConnPool::SqlConnPool() {

}

SqlConnPool::SqlConnPool(SqlConnPool *obj)
{

}

SqlConnPool::~SqlConnPool() {
    ClosePool();
}

bool SqlConnPool::Init(const std::string& host, int port, const std::string& user,
                       const std::string& pwd, const std::string& db_name, unsigned long conn_size) {
    for (unsigned long i = 0; i < conn_size; ++i) {
        MYSQL* sql = nullptr;
        sql = mysql_init(sql);
        std::unique_ptr<SqlOps> conn (new SqlOps());
        if (!conn->connect(host.c_str(), port, user.c_str(), pwd.c_str(), db_name.c_str())) {
            return false;
        }
        m_connQue.push(std::move(conn));
    }
    m_max_conn = conn_size;
    return true;
}

// 连接池在释放时，需要先清空整个队列,再关闭数据库连接
void SqlConnPool::ClosePool() {
    std::unique_lock<std::mutex> locker(m_mutex);
    while (!m_connQue.empty()) {
        auto item = std::move(m_connQue.front());
        m_connQue.pop();
        DestoryConnection(std::move(item));
    }
}

// 获取数据库对象，即从整个数据连接队列中取出一个对象
std::unique_ptr<SqlOps> SqlConnPool::getConnObj() {
    std::unique_ptr<SqlOps> sql;
    if (m_connQue.empty()) {
        LOG_WARN("SqlConnPool busy");
        return nullptr;
    }

    {
        std::unique_lock<std::mutex> locker(m_mutex);
        sql = std::move(m_connQue.front());
        m_connQue.pop();
    }
    return sql;
}

// 释放，就是将连接送回原先的连接队列中
void SqlConnPool::FreeConnObj(std::unique_ptr<SqlOps> conn) {
    if (!conn) {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_connQue.push(std::move(conn));
    }
}

void SqlConnPool::DestoryConnection(std::unique_ptr<SqlOps> conn)
{
    if (conn) {
        conn->CloseConnect();
    }
}
