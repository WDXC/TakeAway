#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

#include "TcpServer.hpp"
#include <Sql/SqlHandler.hpp>

class HttpRequest;
class HttpResponse;

class HttpServer : NoCopyable {
    public:
        using HttpCallback = std::function<void (const HttpRequest&, HttpResponse*)>;

    public:
        HttpServer(EventLoop* loop,
                   const InetAddress& listAddr,
                   const std::string& name,
                   SqlHandler &obj,
                   TcpServer::Option option = TcpServer::k_reuse_port);

        void setHttpCallback(const HttpCallback& cb) {
            httpCallback_ = cb;
        }

        void setThreadNum(int num) {
            server_.set_thread_num(num);
        }

        void start();

    private:
        void onConnection(const TcpConnectionPtr& conn);
        void onMessage(const TcpConnectionPtr& conn, Buffer* buf, TimeStamp receive);
        void onRequest(const TcpConnectionPtr& conn, const HttpRequest&);

    private:
        TcpServer server_;
        SqlHandler sql_obj;
        HttpCallback httpCallback_;
};

#endif
