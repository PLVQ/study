#ifndef _SERVER_
#define _SERVER_
#include "easy_tcp_server.h"

class Server : public EasyTcpServer
{
private:
    /* data */
public:
    Server(/* args */);
    ~Server();
    virtual void clientJoin();
    virtual void clientLeave();
    virtual void onNetMsg(ClientSocket *pCLient, dataHeader *header);
    virtual void onNetRecv();
};

#endif