#ifndef EASY_TCP_CLIENT_HPP
#define EASY_TCP_CLIENT_HPP

#include "public_header.h"
#include "client_message.h"
#include "cell_timestamp.h"

#define RECV_MAX_SIZE 10240

class EasyTcpClient
{
private:
    SOCKET m_sock;
    char m_szRecv[RECV_MAX_SIZE] = {};
    char m_szMsg[RECV_MAX_SIZE * 10] = {};
    int m_lastPos = 0;

public:
    EasyTcpClient();
    virtual ~EasyTcpClient();

    // 初始化socket
    bool initSocket();
    // 连接服务器
    bool Connect(char *ip, short int port);
    // 清理socket
    void Close();
    // 检测socket
    bool isRun();
    // 处理socket
    bool onRun();
    // 接收数据
    int recvData();
    // 处理消息
    int onNetMsg(dataHeader *header);
    // 发送数据
    int sendData(dataHeader *header);
    //
    int getSock() { return m_sock; }
};

#endif