#ifndef HTTPRESPONSE_H_
#define HTTPRESPONSE_H_

#include <map>
#include "Http/HttpDataHandle.hpp"
#include <Sql/SqlHandler.hpp>
#include <memory>

class Buffer;

class HttpResponse  {
    public:
        enum HttpStatusCode {
            kUnknown,
            k200Ok = 200,
            k301MovedPermanently = 301,
            k400BadRequest = 400,
            k404NotFound = 404,
        };

        explicit HttpResponse(bool close, SqlHandler& conn) :
            statusCode_(kUnknown),
            closeConnection_(close),
            obj(conn){
        }

        void setStatusCode(HttpStatusCode code) {
            statusCode_ = code;
        }

        void setStatusMessage(const std::string& message) {
            statusMessage_ = message;
        }

        void setCloseConnection(bool on) {
            closeConnection_ = on;
        }

        bool getcloseConnection() const {
            return closeConnection_;
        }

        void addHeader(const std::string& key, const std::string& value) {
            headers_[key] = value;
        }

        void setContentType(const std::string& contentType) {
            addHeader("Content-Type", contentType);
        }

        void setBody(const std::string& body) {
            body_ = body;
        }

        std::string dealData(Json::CJsonData& json_data, std::string& method) {
            return obj.messageProc(json_data, method);
        }
        void appendToBuffer(Buffer* output) const ;

    private:
        std::map<std::string, std::string> headers_;
        HttpStatusCode statusCode_;
        std::string statusMessage_;
        bool closeConnection_;
        std::string body_;
        HttpDataHandle obj;
};


#endif
