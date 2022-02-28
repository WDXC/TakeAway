
#ifndef HTTPDATAHANDLE_H_
#define HTTPDATAHANDLE_H_

#include "Json_data.hpp"
#include "Http/HttpCallback.hpp"
#include "Log.hpp"
#include <memory>

class HttpDataHandle {
public:
    HttpDataHandle();
    ~HttpDataHandle();

public:
    void messageProc(Json::CJsonData& json_data);

private:
    /* 测试数据 */
    void handleAdd(Json::CJsonData& json_data);
    void handleMul(Json::CJsonData& json_data);

private:
    std::unique_ptr<HttpCallback> obj;
};



#endif
