#include <string.h>
#include <iostream>
#include <map>
#include "Http/HttpServer.hpp"
#include "Http/HttpContext.hpp"
#include "Http/HttpRequest.hpp"
#include "Http/HttpResponse.hpp"
#include "Http/HttpDataHandle.hpp"
#include <Sql/SqlHandler.hpp>
#include "TimeStamp.hpp"
#include "Json_data.hpp"

using namespace std;

bool benchmark = false;

void onRequest(const HttpRequest& req, HttpResponse* resp) {
    std::cout << "Headers " << req.methodString() << " " << req.GetPath()
              << std::endl;

    if (!benchmark)
    {
        const std::map<string, string>& headers = req.Header();
        for (const auto& header : headers)
        {
            std::cout << header.first << ": " << header.second << std::endl;
        }
    }
    if (req.GetPath() == "/") {
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->addHeader("Server", "Zty");
//        resp->addHeader("Access-Control-Allow-Origin", "*");
//        resp->addHeader("Access-Control-Allow-Headers", "Content-Type");
//        resp->addHeader("Cache-Control", "no-cache");
        resp->setContentType("*/*");
    } else if (req.GetPath() == "/api") {
        resp->setContentType("*/*");
        resp->addHeader("Server", "Zty");
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->addHeader("Access-Control-Allow-Headers", "Content-Type");
        resp->addHeader("Cache-Control", "no-cache");
        // 回显 json数据
        Json::CJsonData val = req.getRequestBody();
        resp->setBody(resp->dealData(val));

        // useFetch连通性测试
//        Json::CJsonData root;
//        root.PushValue("title", "succesfully");
//        root.PushValue("body", "hhuj");
//        Json::CJsonData res;
//        res.PushValue(root);
//        std::string strData;
//        res.EncodeArryJson(strData);
//        resp->setBody(strData);
    } else if (req.GetPath() == "/register") {
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setContentType("*/*");
        resp->addHeader("Server", "Zty");
        resp->addHeader("Access-Control-Allow-Origin", "http://127.0.0.1:3000");
        resp->addHeader("Cache-Control", "no-cache");
        resp->addHeader("Access-Control-Allow-Headers", "Content-Type");

        Json::CJsonData root;
        root.PushValue("type", "register");
        root.PushValue("body", "hello world");
        std::string strData;
        root.EncodeJson(strData);
        resp->setBody(strData);
    }
}

int main(int argc, char* argv[]) {
    g_LogMgr::instance()->init("./log", 5000000, 1000);
    EventLoop loop;
    HttpServer server(&loop, InetAddress(8000), "dummy");
    server.setHttpCallback(onRequest);
    server.setThreadNum(8); // 与CPU核心数相同
    server.start();
    loop.loop();
}
