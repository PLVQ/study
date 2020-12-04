#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define FD_SETSIZE 1024
#include <Windows.h>
#include <WinSock2.h>
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define SOCKET int
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR (-1)
#endif

class dataHeader
{
public:
    int dataLen;
    int cmd;
};

enum CMD
{
    LOG_IN,
    LOG_IN_RESPONSE,
    LOG_OUT,
    LOG_OUT_RESPONSE,
    NEW_USER_JOIN
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

class loginResponse : public dataHeader
{
public:
    loginResponse()
    {
        dataLen = sizeof(loginResponse);
        cmd = LOG_IN_RESPONSE;
    }
    char user_name[1024];
};

class logOut : public dataHeader
{
public:
    logOut()
    {
        dataLen = sizeof(logOut);
        cmd = LOG_OUT;
    }
    char user_name[32];
};

class logOutResponse : public dataHeader
{
public:
    logOutResponse()
    {
        dataLen = sizeof(logOutResponse);
        cmd = LOG_OUT_RESPONSE;
    }
    char user_name[32];
};

class newUserJoin : public dataHeader
{
public:
    newUserJoin()
    {
        dataLen = sizeof(newUserJoin);
        cmd = NEW_USER_JOIN;
    }
    int m_socket;
};

class response : public dataHeader
{
public:
    response()
    {
        dataLen = sizeof(response);
        cmd = LOG_IN;
        ret = 0;
    }
    int ret;
    char text[128];
};

int recv_client(SOCKET c_sock);
int send_client(SOCKET c_sock);