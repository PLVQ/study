#ifndef _PUBLIC_HEADER_
#define _PUBLIC_HEADER_

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#ifdef _WIN32
    #define _WINSOCK_DEPRECATED_NO_WARNINGS
    #define WIN32_LEAN_AND_MEAN
    #define FD_SETSIZE 1024
    #include <WinSock2.h>
    #include <Windows.h>
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <string.h>

    #define SOCKET int
    #define INVALID_SOCKET (SOCKET)(~0)
    #define SOCKET_ERROR (-1)
#endif
using namespace std::chrono;

#endif
