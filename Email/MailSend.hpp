#ifndef MAILSEND_H_
#define MAILSEND_H_

#include <string>
#include <netinet/in.h>
#include <Singleton.hpp>

class MailSend {
public:
    MailSend();
    ~MailSend();

public:
    bool init();
    bool SendVerificationCode();
    bool CompareVerification();

private:
    bool sendMsg(char* msg);
    bool recvMsg();
    bool GenerateCode(const std::string& mail);

private:
    static const int BUFSIZE = 256;
    static const int MAXNUM = 999999;
    static const int MINNUM = 100000;

private:
    int cli_sock;
    char message[BUFSIZE];
    std::string VerificationCode;
    struct sockaddr_in cli_addr;

};

using g_mail = Singleton<MailSend>;



#endif
