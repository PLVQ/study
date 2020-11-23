#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <Windows.h>
#include <WinSock2.h>

class dataHeader
{
public:
    int dataLen;
    int cmd;
};
enum CMD {
    LOG_IN,
    LOG_OUT
};
class login : public dataHeader
{
public:
    login()
    {
        dataLen = sizeof(login);
        cmd = LOG_IN;
    }
    char user_name[32];
    char passwd[32];
};

class response : public dataHeader
{
public:
    response()
    {
        // dataLen = sizeof(response);
        cmd = LOG_IN;
        ret = 0;
    }
    ~response()
    {
        
    }
    int ret;
    std::string text;
};

int send_server(SOCKET c_sock);
int recv_server(SOCKET c_sock);