#ifndef EASY_TCP_SERVER
#define EASY_TCP_SERVER

#include "server_message.h"
#include "cell_timestamp.hpp"

#define RECV_MAX_SIZE 10240

class ClientSocket {
   private:
    SOCKET m_sockfd;
    char m_szMsg[RECV_MAX_SIZE * 10] = {};
    int m_lastPos = 0;

   public:
    ClientSocket(SOCKET sock = INVALID_SOCKET);
    ~ClientSocket();
    SOCKET getSockfd() const;
    char *getSzMsg();
    int getLastPos();

    void setLastPos(int pos);
};

ClientSocket::ClientSocket(SOCKET sock) {
    m_sockfd = sock;
    memset(m_szMsg, 0, sizeof(m_szMsg));
}

ClientSocket::~ClientSocket() {}

SOCKET
ClientSocket::getSockfd() const { return m_sockfd; }

char *ClientSocket::getSzMsg() { return m_szMsg; }

int ClientSocket::getLastPos() { return m_lastPos; }

void ClientSocket::setLastPos(int pos) { m_lastPos = pos; }

class EasyTcpServer {
   private:
    SOCKET m_sock;
    char m_szRecv[RECV_MAX_SIZE];
    std::vector<ClientSocket *> m_clients;
    int count = 0;
    cellTimeStamp m_time;

   public:
    EasyTcpServer();
    virtual ~EasyTcpServer();
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
    bool onRun();
    // 接收数据
    int recvData(ClientSocket *client);
    // 处理消息
    int onNetMsg(SOCKET sock, dataHeader *header);
    // 发送数据
    int sendData(SOCKET sock, dataHeader *header);
    // 广播
    void sendDataAll(dataHeader *header);
};

EasyTcpServer::EasyTcpServer() { m_sock = INVALID_SOCKET; }

EasyTcpServer::~EasyTcpServer() { Close(); }

bool EasyTcpServer::initSocket() {
#ifdef _WIN32
    // windows启动socket
    WORD ver = MAKEWORD(2, 2);
    WSADATA dat;
    WSAStartup(ver, &dat);
#endif
    if (m_sock != INVALID_SOCKET) {
        std::cout << "close last connect socket:" << m_sock << std::endl;
        Close();
    }
    // 1.建立socket
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sock == INVALID_SOCKET) {
        std::cout << "create socket fail!" << std::endl;
        return false;
    }
    std::cout << "create socket success!" << std::endl;
    return true;
}

bool EasyTcpServer::Bind(char *ip, short int port) {
    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;
    _sin.sin_port = htons(8888);
#ifdef _WIN32
    _sin.sin_addr.S_un.S_addr = INADDR_ANY;  // inet_addr("127.0.0.1");
#else
    _sin.sin_addr.s_addr = inet_addr("0.0.0.0");
#endif
    if (bind(m_sock, (sockaddr *)&_sin, sizeof(_sin)) == SOCKET_ERROR) {
        std::cout << "bind addr error!" << std::endl;
        return false;
    }
    std::cout << "bind addr success!" << std::endl;
    return true;
}

bool EasyTcpServer::Listen() {
    if (SOCKET_ERROR == listen(m_sock, 5)) {
        std::cout << "listen port error!" << std::endl;
        return false;
    }
    std::cout << "listen port success!" << std::endl;
    return true;
}

bool EasyTcpServer::Accept() {
    sockaddr_in clientAddr = {};
    int nAddrLen = sizeof(sockaddr_in);
    SOCKET sock = INVALID_SOCKET;
#ifdef _WIN32
    sock = accept(m_sock, (sockaddr *)&clientAddr, &nAddrLen);
#else
    sock = accept(m_sock, (sockaddr *)&clientAddr, (socklen_t *)&nAddrLen);
#endif

    if (sock == INVALID_SOCKET) {
        std::cout << "accept error!" << std::endl;
        return false;
    }
    newUserJoin rsp;
    rsp.m_socket = (int)sock;
    // sendDataAll(&rsp);
    m_clients.push_back(new ClientSocket(sock));
    std::cout << "new client sock<" << sock << "> join!"
              << inet_ntoa(clientAddr.sin_addr) << "clients" << m_clients.size()
              << std::endl;
    return true;
}

void EasyTcpServer::Close() {
    if (m_sock != INVALID_SOCKET) {
        // 4.关闭socket
#ifdef _WIN32
        closesocket(m_sock);
        // windows清理socket
        WSACleanup();
#else
        close(m_sock);
#endif
    }
    m_sock = INVALID_SOCKET;
}

bool EasyTcpServer::isRun() { return m_sock != INVALID_SOCKET; }

bool EasyTcpServer::onRun() {
    fd_set fdRead;
    fd_set fdWrite;
    fd_set fdExp;

    FD_ZERO(&fdRead);
    FD_ZERO(&fdWrite);
    FD_ZERO(&fdExp);

    FD_SET(m_sock, &fdRead);
    FD_SET(m_sock, &fdWrite);
    FD_SET(m_sock, &fdExp);

    SOCKET max_sock = m_sock;

    for (int i = m_clients.size() - 1; i >= 0; --i) {
        FD_SET(m_clients[i]->getSockfd(), &fdRead);
        if (max_sock < m_clients[i]->getSockfd()) {
            max_sock = m_clients[i]->getSockfd();
        }
    }
    timeval tTime = {0, 0};
    int ret = select(max_sock + 1, &fdRead, &fdWrite, &fdExp, &tTime);
    if (ret < 0) {
        std::cout << "select exit!" << std::endl;
        m_sock = INVALID_SOCKET;
        return false;
    }

    if (FD_ISSET(m_sock, &fdRead)) {
        FD_CLR(m_sock, &fdRead);
        Accept();
    }

    for (int i = m_clients.size() - 1; i >= 0; --i) {
        if (FD_ISSET(m_clients[i]->getSockfd(), &fdRead)) {
            if (-1 == recvData(m_clients[i])) {
                auto iter =
                    std::find(m_clients.begin(), m_clients.end(), m_clients[i]);
                if (iter != m_clients.end()) {
                    delete *iter;
                    m_clients.erase(iter);
                }
            }
        }
    }
    return true;
}

int EasyTcpServer::recvData(ClientSocket *client) {
    int nLen = recv(client->getSockfd(), m_szRecv, RECV_MAX_SIZE, 0);
    if (nLen <= 0) {
        std::cout << "client close" << std::endl;
        return -1;
    }
    memcpy(client->getSzMsg() + client->getLastPos(), m_szRecv, nLen);
    client->setLastPos(client->getLastPos() + nLen);
    while (nLen >= sizeof(dataHeader)) {
        dataHeader *header = (dataHeader *)client->getSzMsg();
        if (client->getLastPos() >= header->dataLen) {
            int leftLen = client->getLastPos() - header->dataLen;
            onNetMsg(client->getSockfd(), header);
            memcpy(client->getSzMsg(), client->getSzMsg() + header->dataLen,
                   leftLen);
            client->setLastPos(leftLen);
        } else {
            break;
        }
    }

    return 0;
}

int EasyTcpServer::onNetMsg(SOCKET sock, dataHeader *header) {
    count++;
    if (m_time.getElapsedSecond() >= 1.0) {
        std::cout << "time<" << m_time.getElapsedSecond() << "> recv sock<"
                  << sock << "> clients<" << m_clients.size()
                  << "> msg package<" << count << ">" << std::endl;
        m_time.update();
        count = 0;
    }
    switch (header->cmd) {
        case LOG_IN: {
            login *data = (login *)header;
            loginResponse rsp;
            strcpy(rsp.user_name, data->user_name);
            sendData(sock, &rsp);
        } break;
        case LOG_OUT: {
            logOut *data = (logOut *)header;
            logOutResponse rsp;
            strcpy(rsp.user_name, data->user_name);
            sendData(sock, &rsp);
        } break;
        default:
            break;
    }
    return 0;
}

int EasyTcpServer::sendData(SOCKET sock, dataHeader *header) {
    if (isRun() && header) {
        return send(sock, (char *)header, header->dataLen, 0);
    }
    return 0;
}

void EasyTcpServer::sendDataAll(dataHeader *header) {
    for (int i = m_clients.size() - 1; i >= 0; --i) {
        sendData(m_clients[i]->getSockfd(), header);
    }
}

#endif