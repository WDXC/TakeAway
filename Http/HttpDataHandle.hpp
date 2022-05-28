
#ifndef HTTPDATAHANDLE_H_
#define HTTPDATAHANDLE_H_

#include "Json_data.hpp"
#include "Http/HttpCallback.hpp"
#include "Sql/SqlHandler.hpp"
#include "Log.hpp"
#include <map>
#include <memory>

class HttpDataHandle {
public:
    HttpDataHandle(SqlHandler& sql_handler);
    ~HttpDataHandle();

public:
    std::string messageProc(Json::CJsonData& msg_Body, const std::string& method);
    std::string messageGet(Json::CJsonData& msg_body, std::string& method);

private:

    void ApiType(const std::string& type, Json::CJsonData& msg_Body, const std::string &method);
    std::string GeneraterCode();


    // 推送到前端
    void PushToWeb(const std::string& type, Json::CJsonData& msg);

    // 处理GET数据
    void handleGetVal(Json::CJsonData& msg_Body);

    // 用户登陆数据处理
    void handleUserLogin(const std::string &type, Json::CJsonData& msg_Body);
    // 用户注册信息处理
    void handleUserSignup(const std::string &type, Json::CJsonData& msg_Body);
    // 用户邮箱验证处理
    void handleUserMail(const std::string &type, Json::CJsonData& msg_Body);
    // 商家登陆信息处理
    void handleMerchantLogin(const std::string& type, Json::CJsonData& msg_body);
    // 商家注册信息处理
    void handleMerchantSignup(const std::string& type, Json::CJsonData& msg_Body);

    // TEST
    void handleTest(const std::string &type, Json::CJsonData& msg_Body);



private:
    static const int MAXNUM = 999999;
    static const int MINNUM = 100000;

private:
    std::unique_ptr<SqlHandler> sql_obj;
};



#endif
