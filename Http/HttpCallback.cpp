#include <Http/HttpCallback.hpp>

HttpCallback::HttpCallback() : maptable() {

}

HttpCallback::~HttpCallback() {

}

void HttpCallback::init() {
    maptable["loginform"] = LOGIN_;
    maptable["mail"] = MAIL_;
}

void HttpCallback::HandleKeyWord(std::string &type, Json::CJsonData & msg_Body) {
    auto iter = maptable.find(type);
    if (maptable.end() == iter) {
        LOG_ERROR("Search KeyWord error!");
        return;
    }
    Task res = iter->second;
    res(type, msg_Body);
}

void HttpCallback::setLoginCallback(const Task &cb)
{
    LOGIN_ = cb;
}

void HttpCallback::setMailCallback(const Task &cb)
{
    MAIL_ = cb;
}
