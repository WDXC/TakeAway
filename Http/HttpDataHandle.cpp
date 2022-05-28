#include "Http/HttpDataHandle.hpp"
#include "Email/MailSend.hpp"


static std::map<std::string, std::string> usercodemap;
static Json::CJsonData GetVal;

HttpDataHandle::HttpDataHandle(SqlHandler &sql_handler)
{
    sql_obj = std::make_unique<SqlHandler>(sql_handler);
}

HttpDataHandle::~HttpDataHandle()
{
}

std::string HttpDataHandle::messageProc(Json::CJsonData& msg, const std::string& method)
{
    /**
     * json 固定格式：w
     * {
     *      header: {Type: ""}, 用于标识Api类型
     *      body: {} 用于存储数据
     * }
     */
    std::string ans = "";
    Json::CJsonData msg_Head;
    if (!msg.GetValueByName("header", msg_Head)) {
        LOG_ERROR("Get json_data header failed!");
        return ans;
    }

    std::string msg_Type;
    if (!msg_Head.GetValueByName("Type", msg_Type)) {
        LOG_ERROR("Get head type failed!");
        return ans;
    }
    Json::CJsonData msg_Body;
    if (!msg.GetValueByName("body", msg_Body)) {
        LOG_ERROR("Get msg Body failed !");
        return ans;
    }
    ApiType(msg_Type, msg_Body, method);
    msg_Body.EncodeJson(ans);
    return ans;
}


void HttpDataHandle::ApiType(const std::string &type, Json::CJsonData &msg_Body, const std::string& method)
{
    if (method == "GET") {
        handleGetVal(msg_Body);
    }
    if (type == "userloginform") {
        handleUserLogin(type, msg_Body);
    } else if  (type == "usermail") {
        handleUserMail(type, msg_Body);
    } else if (type == "usersignup") {
        handleUserSignup(type, msg_Body);
    } else if (type == "merchantlogin") {
        handleMerchantLogin(type, msg_Body);
    } else if (type == "merchantregister") {
        handleMerchantSignup(type, msg_Body);
    }
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
    GetVal = body;
}

void HttpDataHandle::handleGetVal(Json::CJsonData &msg_Body)
{
    msg_Body.EraseAll();
    msg_Body = GetVal;
}

std::string HttpDataHandle::GeneraterCode()
{
    srand(time(0));
    long num = (rand() % (MAXNUM - MINNUM + 1)) + MINNUM;
    return std::to_string(num);
}

void HttpDataHandle::handleUserLogin(const std::string& type, Json::CJsonData &msg_body) {
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
    const std::string abc = sql_obj->userQuery(email);
    if (!sql_obj->userQuery(email).compare(password)) {
        statusCode_ = "true";
    } else {
        statusCode_ = "false";
    }

    msg_body.EraseAll();
    msg_body.PushValue("login_state", statusCode_);
    PushToWeb(type, msg_body);
}

void HttpDataHandle::handleUserSignup(const std::string &type, Json::CJsonData &msg_Body)
{
    std::string verification;
    std::string email;
    std::string username;
    std::string password;
    std::string codestate = "failed";
    std::string signupstate = "failed";
    if (!msg_Body.GetValueByName("username", username)) {
        LOG_ERROR("dctor email json failed!");
        return;
    }
    if (!msg_Body.GetValueByName("email", email)) {
        LOG_ERROR("dctor email json failed!");
        return;
    }
    if (!msg_Body.GetValueByName("verification", verification)) {
        LOG_ERROR("dctor email json failed!");
        return;
    }
    if (!msg_Body.GetValueByName("password", password)) {
        LOG_ERROR("dctor email json failed!");
        return;
    }
    if (usercodemap[email] == verification) {
        codestate = "success";
        if (sql_obj->userInsert(email, password, username)) {
            signupstate = "success";
        }
    }
    msg_Body.EraseAll();
    msg_Body.PushValue("codestate", codestate);
    msg_Body.PushValue("signupstate", signupstate);
    PushToWeb(type, msg_Body);
}


void HttpDataHandle::handleUserMail(const std::string &type, Json::CJsonData &msg_Body)
{
    std::string email;
    if (!msg_Body.GetValueByName("email", email)) {
        LOG_ERROR("dctor email json failed!");
        return;
    }
    std::string code = GeneraterCode();
    auto it = usercodemap.find(email);
    if (it != usercodemap.end()) {
        usercodemap.erase(it);
    }
    usercodemap.insert(std::pair<std::string, std::string>(email, code));
    std::string codestate;
    MailSend Obj_mail = *g_mail::instance();
    Obj_mail.init();
    Obj_mail.SendVerificationCode(email, code);

    codestate="ok";
    msg_Body.EraseAll();
    msg_Body.PushValue("codestate", codestate);
    PushToWeb(type, msg_Body);
}

void HttpDataHandle::handleMerchantLogin(const std::string &type, Json::CJsonData &msg_body)
{
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
    std::string statusCode_ = "failed";
    const std::string abc = sql_obj->merchantQuery(email);
    if (!sql_obj->userQuery(email).compare(password)) {
        statusCode_ = "success";
    }

    msg_body.EraseAll();
    msg_body.PushValue("merlogin_state", statusCode_);
    PushToWeb(type, msg_body);
}

void HttpDataHandle::handleMerchantSignup(const std::string &type, Json::CJsonData &msg_Body)
{
    std::string email;
    std::string username;
    std::string password;
    std::string signupstate = "failed";
    if (!msg_Body.GetValueByName("username", username)) {
        LOG_ERROR("dctor email json failed!");
        return;
    }
    if (!msg_Body.GetValueByName("email", email)) {
        LOG_ERROR("dctor email json failed!");
        return;
    }
    if (!msg_Body.GetValueByName("password", password)) {
        LOG_ERROR("dctor email json failed!");
        return;
    }

    if (sql_obj->merchantInsert(username, email, password)) {
        signupstate = "success";
    }
    msg_Body.EraseAll();
    msg_Body.PushValue("merchantregister", signupstate);
    PushToWeb(type, msg_Body);
}




void HttpDataHandle::handleTest(const std::string &type, Json::CJsonData &msg_Body)
{
    PushToWeb(type, msg_Body);
}






