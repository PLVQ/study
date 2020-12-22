#include "easy_tcp_server.h"

ClientSocket::ClientSocket(SOCKET sock)
{
    m_sockfd = sock;
    m_lastRecvPos = 0;
    m_lastSendPos = 0;
    memset(m_szMsg, 0, RECV_BUFF_SIZE);
    memset(m_szSendBuff, 0, SEND_BUFF_SIZE);
}

ClientSocket::~ClientSocket()
{
    if (m_sockfd != INVALID_SOCKET)
    {
        // 4.关闭socket
#ifdef _WIN32
        closesocket(m_sockfd);
#else
        close(m_sockfd);
#endif
    }
    m_sockfd = INVALID_SOCKET;
}

SOCKET ClientSocket::getSockfd() const { return m_sockfd; }

char *ClientSocket::getSzMsg() { return m_szMsg; }

int ClientSocket::getLastPos() { return m_lastRecvPos; }

void ClientSocket::setLastPos(int pos) { m_lastRecvPos = pos; }

int ClientSocket::sendData(dataHeader *header)
{
    int ret = SOCKET_ERROR;
    int nSendLen = header->dataLen;
    const char* pSendData = (const char*) header; 
    while (true)
    {
        if (m_lastSendPos + nSendLen >= SEND_BUFF_SIZE)
        {
            int nCopyLen = SEND_BUFF_SIZE - m_lastSendPos;
            memcpy(m_szSendBuff + m_lastSendPos, pSendData, nCopyLen);
            pSendData += nCopyLen;
            nSendLen -= nCopyLen;
            ret = send(m_sockfd, m_szSendBuff, SEND_BUFF_SIZE, 0);
            m_lastSendPos = 0;
            if (ret == SOCKET_ERROR)
            {
                return ret;
            }
        }
        else
        {
            memcpy(m_szSendBuff + m_lastSendPos, pSendData, nSendLen);
            m_lastSendPos += nSendLen;
            break;
        }
    }
    return ret;
}

EasyTcpServer::EasyTcpServer()
{
    m_sock = INVALID_SOCKET;
    m_recvMsgCount = 0;
    m_clientCount = 0;
}

EasyTcpServer::~EasyTcpServer() { Close(); }

void EasyTcpServer::Start()
{
    for (int i = 0; i < CELL_SERVER_COUNT; i++)
    {
        auto server = new cellServer(m_sock);
        m_servers.push_back(server);
        server->Start();
        server->setEvent(this);
    }
}

bool EasyTcpServer::initSocket()
{
    if (m_sock != INVALID_SOCKET)
    {
        std::cout << "close last connect socket:" << m_sock << std::endl;
        Close();
    }
    // 1.建立socket
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sock == INVALID_SOCKET)
    {
        std::cout << "create socket fail!" << std::endl;
        return false;
    }
    return true;
}

bool EasyTcpServer::Bind(char *ip, short int port)
{
    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;
    _sin.sin_port = htons(8888);
#ifdef _WIN32
    _sin.sin_addr.S_un.S_addr = INADDR_ANY; // inet_addr("127.0.0.1");
#else
    _sin.sin_addr.s_addr = inet_addr("0.0.0.0");
#endif
    if (bind(m_sock, (sockaddr *)&_sin, sizeof(_sin)) == SOCKET_ERROR)
    {
        std::cout << "bind addr error!" << std::endl;
        return false;
    }
    std::cout << "bind addr success!" << std::endl;
    return true;
}

bool EasyTcpServer::Listen()
{
    if (SOCKET_ERROR == listen(m_sock, 5))
    {
        std::cout << "listen port error!" << std::endl;
        return false;
    }
    std::cout << "listen port success!" << std::endl;
    return true;
}

bool EasyTcpServer::Accept()
{
    sockaddr_in clientAddr = {};
    int nAddrLen = sizeof(sockaddr_in);
    SOCKET sock = INVALID_SOCKET;
#ifdef _WIN32
    sock = accept(m_sock, (sockaddr *)&clientAddr, &nAddrLen);
#else
    sock = accept(m_sock, (sockaddr *)&clientAddr, (socklen_t *)&nAddrLen);
#endif

    if (sock == INVALID_SOCKET)
    {
        std::cout << "accept error!" << std::endl;
        return false;
    }
    // 将连接的客户端加入cellServer
    addClientToCellServer(new ClientSocket(sock));
    return true;
}

void EasyTcpServer::Close()
{
    if (m_sock != INVALID_SOCKET)
    {
        // 4.关闭socket
#ifdef _WIN32
        closesocket(m_sock);
#else
        close(m_sock);
#endif
    }
    m_sock = INVALID_SOCKET;
}

bool EasyTcpServer::isRun() { return m_sock != INVALID_SOCKET; }

void EasyTcpServer::onRun()
{
    if (isRun())
    {
        time2Msg();
        fd_set fdRead;
        fd_set fdWrite;
        fd_set fdExp;

        FD_ZERO(&fdRead);
        FD_ZERO(&fdWrite);
        FD_ZERO(&fdExp);

        FD_SET(m_sock, &fdRead);
        FD_SET(m_sock, &fdWrite);
        FD_SET(m_sock, &fdExp);

        timeval tTime = {0, 10};
        int ret = select(m_sock + 1, &fdRead, &fdWrite, &fdExp, &tTime);
        // int ret = select(m_sock + 1, &fdRead, &fdWrite, &fdExp, 0);
        if (ret < 0)
        {
            std::cout << "main select exit!" << std::endl;
            m_sock = INVALID_SOCKET;
            return;
        }

        if (FD_ISSET(m_sock, &fdRead))
        {
            FD_CLR(m_sock, &fdRead);
            Accept();
        }
    }
}

void EasyTcpServer::addClientToCellServer(ClientSocket *pClient)
{
    auto pMinServer = m_servers[0];
    for (auto server : m_servers)
    {
        if (pMinServer->getClientCount() > server->getClientCount())
        {
            pMinServer = server;
        }
    }
    pMinServer->joinClients(pClient);
    clientJoin();
}

void EasyTcpServer::time2Msg()
{
    if (m_time.getElapsedSecond() >= 1.0)
    {

        std::cout << "time<" << m_time.getElapsedSecond() << ">, clients<" << m_clientCount
                  << ">, msg package<" << m_recvMsgCount << ">" << std::endl;
        m_time.update();
        m_recvMsgCount = 0;
    }
}

void EasyTcpServer::clientJoin()
{
    m_clientCount++;
}

void EasyTcpServer::clientLeave()
{
    m_clientCount--;
}

void EasyTcpServer::onNetMsg()
{
    m_recvMsgCount++;
}

cellServer::cellServer(SOCKET sock)
{
    m_sock = sock;
    m_thread = nullptr;
    m_recvCount = 0;
    m_event = nullptr;
}

cellServer::~cellServer()
{
    Close();
    m_sock = INVALID_SOCKET;
    delete m_event;
    delete m_thread;
}

void cellServer::Start()
{
    m_thread = new std::thread(std::mem_fn(&cellServer::onRun), this);
}

void cellServer::setEvent(ClientLeaveEvent *cEvent)
{
    m_event = cEvent;
}

void cellServer::Close()
{
    if (m_sock != INVALID_SOCKET)
    {
        // 4.关闭socket
#ifdef _WIN32
        for (int n = (int)m_clients.size() - 1; n >= 0; n--)
        {
            closesocket(m_clients[n]->getSockfd());
        }
        closesocket(m_sock);
#else
        for (int n = (int)m_clients.size() - 1; n >= 0; n--)
        {
            close(m_clients[n]->getSockfd());
        }
        close(m_sock);
#endif
    }
    m_sock = INVALID_SOCKET;
}

bool cellServer::isRun() { return m_sock != INVALID_SOCKET; }

void cellServer::onRun()
{
    m_clientsChange = true;
    while (isRun())
    {
        if (m_clientBuff.size() > 0)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            for (auto client : m_clientBuff)
            {
                m_clients[client->getSockfd()] = client;
            }
            m_clientBuff.clear();
            m_clientsChange = true;
        }
        if (m_clients.empty())
        {
            std::chrono::milliseconds tMilli(1);
            std::this_thread::sleep_for(tMilli);
            continue;
        }
        fd_set fdRead;

        FD_ZERO(&fdRead);
        if (m_clientsChange)
        {
            m_maxSock = m_clients.begin()->second->getSockfd();

            for (auto iter : m_clients)
            {
                FD_SET(iter.second->getSockfd(), &fdRead);
                if (m_maxSock < iter.second->getSockfd())
                {
                    m_maxSock = iter.second->getSockfd();
                }
            }
            memcpy(&m_fdReadBak, &fdRead, sizeof(fd_set));
            m_clientsChange = false;
        }
        else
        {
            memcpy(&fdRead, &m_fdReadBak, sizeof(fd_set));
        }
        timeval tTime = {1, 0};
        int ret = select(m_maxSock + 1, &fdRead, nullptr, nullptr, &tTime);
        // int ret = select(max_sock + 1, &fdRead, nullptr, nullptr, 0);
        if (ret < 0)
        {
            std::cout << "select exit!" << std::endl;
            m_sock = INVALID_SOCKET;
            return;
        }
        else if (ret == 0)
        {
            continue;
        }
#ifdef _WIN32
        for (int n = 0; n < fdRead.fd_count; n++)
        {
            auto iter  = m_clients.find(fdRead.fd_array[n]);
            if (iter != m_clients.end())
            {
                if (-1 == recvData(iter->second))
                {
                    m_event->clientLeave();
                    m_clientsChange = true;
                    m_clients.erase(iter->first);
                }
            }else {
                printf("error. if (iter != _clients.end())\n");
            }

        }
#else
        std::vector<ClientSocket *> temp;
        for (auto iter : m_clients)
        {
            if (FD_ISSET(iter.second->getSockfd(), &fdRead))
            {
                // FD_CLR(m_clients[i]->getSockfd(), &fdRead);
                if (-1 == recvData(iter.second))
                {
                    m_event->clientLeave();
                    m_clientsChange = true;
                    temp.push_back(iter.second);
                }
            }
        }
        for (auto pClient : temp)
        {
            m_clients.erase(pClient->getSockfd());
            delete pClient;
        }
#endif
    }
}

int cellServer::recvData(ClientSocket *pClient)
{
    // 接收客户端数据
    char *szRecv = pClient->getSzMsg() + pClient->getLastPos();
    int nLen = recv(pClient->getSockfd(), szRecv, (RECV_BUFF_SIZE) - pClient->getLastPos(), 0);
    if (nLen <= 0)
    {
        return -1;
    }
    // memcpy(pClient->getSzMsg() + pClient->getLastPos(), m_szRecv, nLen);
    pClient->setLastPos(pClient->getLastPos() + nLen);
    while (nLen >= sizeof(dataHeader))
    {
        dataHeader *header = (dataHeader *)pClient->getSzMsg();
        if (pClient->getLastPos() >= header->dataLen)
        {
            int leftLen = pClient->getLastPos() - header->dataLen;
            onNetMsg(pClient, header);
            memcpy(pClient->getSzMsg(), pClient->getSzMsg() + header->dataLen,
                   leftLen);
            pClient->setLastPos(leftLen);
        }
        else
        {
            break;
        }
    }

    return 0;
}

int cellServer::onNetMsg(ClientSocket *pCLient, dataHeader *header)
{
    m_event->onNetMsg();
    switch (header->cmd)
    {
    case LOG_IN:
    {
        login *data = (login *)header;
        loginResponse rsp;
        strcpy(rsp.user_name, data->user_name);
        pCLient->sendData(&rsp);
    }
    break;
    case LOG_OUT:
    {
        logOut *data = (logOut *)header;
        logOutResponse rsp;
        strcpy(rsp.user_name, data->user_name);
        pCLient->sendData(&rsp);
    }
    break;
    default:
        break;
    }
    return 0;
}

void cellServer::sendDataAll(dataHeader *header)
{
    for (int i = m_clients.size() - 1; i >= 0; --i)
    {
        m_clients[i]->sendData(header);
    }
}

void cellServer::joinClients(ClientSocket *pClient)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_clientBuff.push_back(pClient);
}

int cellServer::getClientCount()
{
    return m_clients.size() + m_clientBuff.size();
}