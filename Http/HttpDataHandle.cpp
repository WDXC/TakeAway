#include "Http/HttpDataHandle.hpp"

HttpDataHandle::HttpDataHandle() :
    obj(new HttpCallback())
{
    connpool_ = std::make_unique<SqlConnPool>(g_sqlMgr::instance());
    connpool_->Init("127.0.0.1", 3306, "root", "password", "TakeAway", 100);
    sql_obj = std::make_unique<SqlHandler>(std::move(connpool_));
    // 登陆接口
    obj->setLoginCallback(std::bind(&HttpDataHandle::handleLogin, this, std::placeholders::_1,
                                    std::placeholders::_2));
}

HttpDataHandle::~HttpDataHandle()
{

}

void HttpDataHandle::messageProc(Json::CJsonData& msg)
{
    /**
     * json 固定格式
     * {
     *      header: {Type: ""}, 用于标识Api类型
     *      body: {} 用于存储数据
     * }
     */
    Json::CJsonData msg_Head;
    if (!msg.GetValueByName("header", msg_Head)) {
        LOG_ERROR("Get json_data header failed!");
        return;
    }

    std::string msg_Type;
    if (!msg_Head.GetValueByName("Type", msg_Type)) {
        LOG_ERROR("Get head type failed!");
        return;
    }
    Json::CJsonData msg_Body;
    if (!msg.GetValueByName("body", msg_Body)) {
        LOG_ERROR("Get msg Body failed !");
        return;
    }
    obj->init();
    obj->HandleKeyWord(msg_Type, msg_Body);
    msg.EraseAll();
    std::string res;
    msg_Body.EncodeJson(res);
    msg = msg_Body;
}

void HttpDataHandle::PushToWeb(const std::string &type, Json::CJsonData &msg)
{
    Json::CJsonData header;
    Json::CJsonData body;
    header.PushValue("Type", type);
    body.PushValue("header", header);
    body.PushValue("body", msg);
    msg.EraseAll();
    msg = body;
}

void HttpDataHandle::handleLogin(const std::string& type, Json::CJsonData &msg_body) {
    std::string email;
    if (!msg_body.GetValueByName("email", email)) {
        LOG_ERROR("dctor email json failed!");
        return;
    }
    std::string password;
    if (!msg_body.GetValueByName("password", password)) {
        LOG_ERROR("dctor email json failed!");
        return;
    }

    std::string statusCode_;
    if (!sql_obj->dealQuery(email).compare(password)) {
        statusCode_ = "true";
    } else {
        statusCode_ = "false";
    }

    msg_body.EraseAll();
    msg_body.PushValue("login_state", statusCode_);
    PushToWeb(type, msg_body);
}
