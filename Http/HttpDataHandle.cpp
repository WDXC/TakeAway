#include "Http/HttpDataHandle.hpp"

HttpDataHandle::HttpDataHandle() :
    obj(new HttpCallback()) {
    obj->setAddCallback(std::bind(&HttpDataHandle::handleAdd, this, std::placeholders::_1));
    obj->setMuiCallback(std::bind(&HttpDataHandle::handleMul, this, std::placeholders::_1));
}

HttpDataHandle::~HttpDataHandle()
{

}

void HttpDataHandle::messageProc(Json::CJsonData& json_data)
{
    std::string msgType;
    if (!json_data.GetValueByName("Type", msgType)) {
        LOG_ERROR("get json_data type failed");
        return;
    }
    obj->init();
    obj->HandleKeyWord(msgType, json_data);
}

void HttpDataHandle::handleAdd(Json::CJsonData &json_data) {
    int rhs, lhs;
    json_data.GetValueByName("lhs", lhs);
    json_data.GetValueByName("rhs", rhs);
    int res = lhs + rhs;
    json_data.EraseAll();
    json_data.PushValue("result", std::to_string(res));
}

void HttpDataHandle::handleMul(Json::CJsonData &json_data) {
    int rhs, lhs;
    json_data.GetValueByName("lhs", lhs);
    json_data.GetValueByName("rhs", rhs);
    int res = lhs * rhs;
    json_data.EraseAll();
    json_data.PushValue("result", std::to_string(res));
}
