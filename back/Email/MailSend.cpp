#include <Email/MailSend.hpp>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <cstdlib>
#include <unistd.h>

MailSend::MailSend()
    : cli_sock(0),
    cli_addr(),
    codeMap()
{

}

MailSend::~MailSend()
{
    if (cli_sock) {
        ::close(cli_sock);
    }
}

bool MailSend::init()
{
    const std::string hostIp = "120.241.186.196";
    const int port = 25;
    cli_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (cli_sock < 0) {
        return false;
    }
    int on = 1;
    memset(&cli_sock, 0x0, sizeof(cli_addr));
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(port);
    inet_pton(AF_INET, hostIp.c_str(), &cli_addr.sin_addr);
//    setsockopt(cli_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    int ret = connect(cli_sock, (struct sockaddr*)&cli_addr, sizeof(cli_addr));
    if (ret < 0) {
        int k = errno;
        return false;
    }
    return true;
}

bool MailSend::SendVerificationCode(const std::string &mail, std::string code)
{
    sendMsg("HELO qq.com\r\n");
    recvMsg();
    // 验证登陆
    sendMsg("auth login\r\n");
    recvMsg();
    // 用户名(bash64编码)
    sendMsg("MTUwODQ5ODEwOEBxcS5jb20=");
    sendMsg("\r\n");
    recvMsg();
    // 密码(bash64编码)
    sendMsg("aHFzbGJyb2V2bm5rYmFhYQ==");
    sendMsg("\r\n");
    recvMsg();

    // 发送方邮箱配置
    sendMsg("mail from: <");
    sendMsg("1508498108@qq.com");
    sendMsg(">");
    sendMsg("\r\n");
    recvMsg();

    // 接收方邮箱配置
    sendMsg("rcpt to: <");
    sendMsg(mail.c_str());
    sendMsg(">");
    sendMsg("\r\n");
    recvMsg();

    // 开始发送邮件内容
    sendMsg("data\r\n");
    sendMsg("subject: Verfication Code\r\n\r");

    // 发送的生成的六位验证码
    sendMsg(code.c_str());
    sendMsg("\r\n.\r\n");
    recvMsg();

    // 结束
    sendMsg("QUIT\r\n");
    recvMsg();
    return true;
}

bool MailSend::handleSend(std::string &mail, std::string &code) {
    init();
    return SendVerificationCode(mail, code);
}

bool MailSend::sendMsg(const char *msg)
{
    int ret = send(cli_sock, &message, strlen(message), 0);
    if (ret < 0) {
        return false;
    }
    memset(&message, 0x0, BUFSIZE);
    return true;
}

bool MailSend::recvMsg()
{
    memset(&message, 0x0, BUFSIZE);
    int ret = recv(cli_sock, &message, BUFSIZE, 0);
    if (ret < 0) {
        return false;
    }
    return true;
}
