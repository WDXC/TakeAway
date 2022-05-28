#ifndef MAILSEND_H_
#define MAILSEND_H_

#include <map>
#include <string>
#include <memory>
#include <functional>
#include <netinet/in.h>
#include <Singleton.hpp>
#include <ThreadObject.hpp>

class MailSend {

public:
    MailSend();
    ~MailSend();

public:
    bool init();
    bool SendVerificationCode(const std::string& mail, std::string code);
    bool handleSend(std::string& mail, std::string& code);

private:
    bool sendMsg(const char* msg);
    bool recvMsg();

private:
    static const int BUFSIZE = 256;
private:
    int cli_sock;
    char message[BUFSIZE];
    struct sockaddr_in cli_addr;
    std::map<std::string, std::string> codeMap;
};

using g_mail = Singleton<MailSend>;

#endif
