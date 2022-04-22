
#ifndef HTTPDATAHANDLE_H_
#define HTTPDATAHANDLE_H_

#include "Json_data.hpp"
#include "Http/HttpCallback.hpp"
#include "Sql/SqlHandler.hpp"
#include "Log.hpp"
#include <memory>

class HttpDataHandle {
public:
    HttpDataHandle(SqlHandler& sql_handler);
    ~HttpDataHandle();

public:
    void messageProc(Json::CJsonData& msg_Body);

private:

    // 推送到前端
    void PushToWeb(const std::string& type, Json::CJsonData& msg);

    // 登陆数据处理
    void handleLogin(const std::string &type, Json::CJsonData& msg_Body);
    void handleMail(const std::string& mail);


private:
    std::unique_ptr<HttpCallback> obj;
    std::unique_ptr<SqlHandler> sql_obj;
};



#endif
