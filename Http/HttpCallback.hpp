#ifndef HTTPCALLBACK_H_
#define HTTPCALLBACK_H_

#include <functional>
#include <Json_data.hpp>
#include <Log.hpp>
#include <map>

class HttpCallback {
public:
    using Task = std::function<void(Json::CJsonData&)>;

public:
    HttpCallback() : maptable() {}
    ~HttpCallback() {}

    void init() {
        maptable["add"] = Add_;
        maptable["mul"] = Mul_;
    }

    void HandleKeyWord(std::string& type,Json::CJsonData& json_data) {
        std::map<std::string, Task>::const_iterator iter = maptable.find(type);
        if (maptable.end() == iter) {
            LOG_ERROR("Search key word error!");
            return;
        }
        Task res = iter->second;
        res(json_data);
    }

    /* 测试数据 */
    void setAddCallback(const Task& cb) {
        Add_ = cb;
    }

    void setMuiCallback(const Task& cb) {
        Mul_ = cb;
    }

private:
    Task Add_;
    Task Mul_;
    std::map<std::string, Task> maptable;
};


#endif
