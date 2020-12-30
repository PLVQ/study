#include "server.h"

Server::Server()
{
}

Server::~Server()
{
}

void Server::clientJoin()
{
    m_clientCount++;
}

void Server::clientLeave()
{
    m_clientCount--;
}

void Server::onNetMsg(cellServer* pCellServer, ClientSocket *pCLient, dataHeader *header)
{
    m_recvMsgCount++;
    switch (header->cmd)
    {
    case LOG_IN:
    {
        login *data = (login *)header;
        loginResponse *rsp = new loginResponse();
        strcpy(rsp->user_name, data->user_name);
        pCellServer->addSendTask(pCLient, rsp);
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
}

void Server::onNetRecv()
{
    m_recvCount++;
}