#include <string.h>
#include <string>
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

void setHeader(HttpResponse* resp) {
    resp->setStatusCode(HttpResponse::k200Ok);
    resp->setContentType("*/*");
    resp->addHeader("Server", "Zty");
    resp->addHeader("Access-Control-Allow-Origin", "*");
    resp->addHeader("Access-Control-Allow-Headers", "Content-Type, x-requested-with");
    resp->addHeader("Cache-Control", "no-cache");
    resp->addHeader("Access-Control-Allow-Methods", "*");}

void onRequest(const HttpRequest& req, HttpResponse* resp) {
    std::cout << "Headers " << req.methodString() << " " << req.GetPath()
              << std::endl;
    std::string path = req.methodString();

    if (!benchmark)
    {
        const std::map<string, string>& headers = req.Header();
        for (const auto& header : headers)
        {
            std::cout << header.first << ": " << header.second << std::endl;
        }
    }

    if (req.GetPath() == "/") {
        setHeader(resp);
    } else if (req.GetPath() == "/userlogin") {
        setHeader(resp);
        Json::CJsonData val = req.getRequestBody();
        resp->setBody(resp->dealData(val, path));
        return;
    } else if (req.GetPath() == "/usermail") {
        setHeader(resp);
        Json::CJsonData val = req.getRequestBody();
        resp->setBody(resp->dealData(val, path));
        return;
    } else if (req.GetPath() == "/usersignup") {
        setHeader(resp);
        Json::CJsonData val = req.getRequestBody();
        resp->setBody(resp->dealData(val, path));
        return;
    } else if (req.GetPath() == "/merchantlogin") {
        setHeader(resp);
        Json::CJsonData val = req.getRequestBody();
        resp->setBody(resp->dealData(val, path));
        return;
    } else if (req.GetPath() == "/merchatregister") {
        setHeader(resp);
        Json::CJsonData val = req.getRequestBody();
        resp->setBody(resp->dealData(val, path));
    } else if (req.GetPath() == "/merchantregister") {
        setHeader(resp);
        Json::CJsonData val = req.getRequestBody();
        resp->setBody(resp->dealData(val, path));
        return;
    }
}

int main() {
    g_LogMgr::instance()->init("./log", 5000000, 1000);
    SqlConnPool* obj = g_sqlMgr::instance();
    if (!obj->Init("127.0.0.1", 3306, "root", "password", "TakeAway", 10)) {
        return -1;
    }
    SqlHandler* sql_obj = new SqlHandler(obj);
    EventLoop loop;
    HttpServer server(&loop, InetAddress(8000), "dummy", *sql_obj);
    server.setHttpCallback(onRequest);
    server.setThreadNum(3); // 与CPU核心数相同
    server.start();
    loop.loop();
}
