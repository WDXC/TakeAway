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
    HttpCallback();
    ~HttpCallback();

    // 初始化接口驱动表 -> 初始化maptable
    void init();
    // 根据传入的类型，选择不同的回调函数进行处理
    void HandleKeyWord(std::string& type,Json::CJsonData& json_data);

    /* 测试数据 */
    void setAddCallback(const Task& cb);
    void setMuiCallback(const Task& cb);

private:
    Task Add_;
    Task Mul_;
    std::map<std::string, Task> maptable;
};


#endif
