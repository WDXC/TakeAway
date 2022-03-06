#ifndef SQLOPS_H_
#define SQLOPS_H_

#include <string>
#include <string.h>
#include <map>
#include <mysql/mysql.h>

class SqlOps {
public:
    SqlOps();
    ~SqlOps();

    bool connect(const char* ip, int port, const char* username, const char* password, const char* db_name);
    void CloseConnect();

    /***************************/
    //        普通处理
    /***************************/

    // 执行查询
    bool ExecuteQuery(const char* sql);
    bool Execute(const char* sql);
    // 开启事务
    bool startTransaction();
    // 0-rollback 1-commit
    bool EndTransaction();
    // 获取行数
    int GetQueryResultCount();
    // 根据传入的名字获取字段值
    char *GetField(const char* fieldname);
    // 根据列的序号查询，得到该列的值
    char *GetField(int fieldnum);
    // 拿到一行的结果
    char **FetchRow();
    // 获取conn
    MYSQL &getConn();
    // 转义字符，避免特殊字符
    void EscapeString(char* dst, char* src);



     /***************************/
    //        预处理
    /***************************/


    // 获取MYSQL_STMT句柄
    MYSQL_STMT &getStmt();
    // 判断其是否为预处理语句
    bool initSql(MYSQL_STMT& stmt, std::string& sql);
    // bind (int)类型
    bool set_param_bind(int index, int& value);
    // bind (string)类型
    bool set_param_bind(int index,const std::string& value);
    //  为主键提供null类型
    bool set_param_null(int index);
    // 处理结果
    bool set_param_result(int index, enum_field_types type, char* buffer,
                        unsigned long buffer_len, unsigned long * len);
    // 语句执行
    bool pre_excute();
    // 查询
    bool query();
    // 存储结果
    bool get_result();
    // 获取结果
    bool fetch_result();
    // 释放结果
    void free();

private:
    // 重连
    bool ReConnect();
    // 释放上一次查询结果
    void FreeResult();

private:
    bool isConnected;                           // 是否连接
    bool bCommit;                               // 选择事务类型，commit/rollback
    bool reconnected;                           // 重连标识
    int fieldcnt;                               // 查询列数
    std::map<std::string, int> mapFieldToIndex; // 列名与序列对应

    MYSQL sql_conn;                             // 数据库连接
    MYSQL_ROW sql_row;                          // 结果中的当前行
    MYSQL_RES* sql_result;                      // 查询结果

    MYSQL_STMT *sql_stmt;                       // 预处理
    MYSQL_BIND *m_param_bind;
    MYSQL_BIND *m_result_bind;
    MYSQL_RES  *sql_stmt_result;
    unsigned long m_param_count;
    unsigned int m_result_count;


    // 保存如下信息，为了重连
    std::string hostIp_;
    int port_;
    std::string username_;
    std::string password_;
    std::string db_name_;

};




#endif
