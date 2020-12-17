#ifndef _PUBLIC_HEADER_
#define _PUBLIC_HEADER_

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <mutex>
#include <thread>
#include <atomic>
#include <functional>

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

using namespace std::chrono;

#endif