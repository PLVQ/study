#ifndef _EASY_TCP_SERVER_
#define _EASY_TCP_SERVER_

#include "server_message.h"
#include "cell_timestamp.h"
#include "cell_task.h"

#ifndef RECV_BUFF_SIZE

#define RECV_BUFF_SIZE 10240 * 5
#define SEND_BUFF_SIZE 10240 * 5

#endif

#define CELL_SERVER_COUNT 4

class ClientSocket
{
private:
    SOCKET m_sockfd;
    char m_szMsg[RECV_BUFF_SIZE];
    char m_szSendBuff[SEND_BUFF_SIZE];
    int m_lastRecvPos;
    int m_lastSendPos;

public:
    ClientSocket(SOCKET sock = INVALID_SOCKET);
    ~ClientSocket();
    SOCKET getSockfd() const;
    char *getSzMsg();
    int getLastPos();

    void setLastPos(int pos);
    // 发送数据
    int sendData(dataHeader *header);
};

class EasyTcpServer;
class cellServer;

class ClientLeaveEvent
{
public:
    virtual ~ClientLeaveEvent(){}
    virtual void clientJoin() = 0;
    virtual void clientLeave() = 0;
    virtual void onNetMsg(cellServer* pCellServer, ClientSocket *pCLient, dataHeader *header) = 0;
    virtual void onNetRecv() = 0;
private:
};

class CellSendMsg2ClientTask:public CellTask
{
private:
    ClientSocket* m_pClient;
    dataHeader* m_pHeader;
public:
    CellSendMsg2ClientTask(ClientSocket* pClient, dataHeader* pHeader);
    virtual ~CellSendMsg2ClientTask();
    virtual void doTask(); 
};

class cellServer
{
private:
    SOCKET m_sock;
    std::map<SOCKET, ClientSocket*> m_clients;
    std::vector<ClientSocket*> m_clientBuff;
    char m_szRecv[RECV_BUFF_SIZE];
    std::mutex m_mutex;
    std::thread *m_thread;
    ClientLeaveEvent *m_event;
    fd_set m_fdReadBak;
    bool m_clientsChange;
    SOCKET m_maxSock;
    CellTaskServer m_taskServer;

public:
    std::atomic_int m_recvCount;
public:
    cellServer(SOCKET sock = INVALID_SOCKET);
    ~cellServer();
    void Start();
    void setEvent(ClientLeaveEvent *cEvent);
    // 清理socket
    void Close();
    // 检测socket
    bool isRun();
    // 处理socket
    void onRun();
    // 接收数据
    int recvData(ClientSocket *pClient);
    // 处理消息
    virtual void onNetMsg(ClientSocket *pClient, dataHeader *header);
    // 发送数据
    int sendData(SOCKET sock, dataHeader *header);
    // 广播
    void sendDataAll(dataHeader *header);
    // 客户端加入
    void joinClients(ClientSocket* pClient);
    // 获取客户端数量
    int getClientCount();
    // 添加发送任务
    void addSendTask(ClientSocket* pClient, dataHeader* pHeader);
};

class EasyTcpServer:public ClientLeaveEvent
{
private:
    SOCKET m_sock;
    std::vector<cellServer*> m_servers;
    cellTimeStamp m_time;
protected:
    std::atomic_int m_clientCount;
    std::atomic_int m_recvMsgCount;
    std::atomic_int m_recvCount;
public:
    EasyTcpServer();
    virtual ~EasyTcpServer();
    void Start();
    // 初始化socket
    bool initSocket();
    // 绑定地址和端口
    bool Bind(char *ip, short int port);
    // 监听端口
    bool Listen();
    // 接受连接
    bool Accept();
    // 清理socket
    void Close();
    // 检测socket
    bool isRun();
    // 处理socket
    void onRun();
    // 
    void addClientToCellServer(ClientSocket* pCLient);
    //
    void time2Msg();
    //
    virtual void clientJoin();
    virtual void clientLeave();
    virtual void onNetMsg(cellServer* pCellServer, ClientSocket *pCLient, dataHeader *header);
    virtual void onNetRecv();
};

#endif