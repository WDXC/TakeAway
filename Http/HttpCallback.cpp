#include <Http/HttpCallback.hpp>

HttpCallback::HttpCallback() : maptable() {

}

HttpCallback::~HttpCallback() {

}

void HttpCallback::init() {
    maptable["add"] = Add_;
    maptable["mul"] = Mul_;
}

void HttpCallback::HandleKeyWord(std::string &type, Json::CJsonData &json_data) {
    auto iter = maptable.find(type);
    if (maptable.end() == iter) {
        LOG_ERROR("Search KeyWord error!");
        return;
    }
    Task res = iter->second;
    res(json_data);
}

void HttpCallback::setAddCallback(const Task &cb) {
    Add_ = cb;
}

void HttpCallback::setMuiCallback(const Task &cb) {
    Mul_ = cb;
}
