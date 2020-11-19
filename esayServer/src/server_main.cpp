#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <WinSock2.h>

#include "message.h"

using namespace std;

// #pragma comment(lib, "ws2_32.lib")
int main()
{
	// 启动socket
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	
	// 1.建立一个socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 2.bind
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(8888);
	_sin.sin_addr.S_un.S_addr = INADDR_ANY; // inet_addr("127.0.0.1");
	if (bind(_sock, (sockaddr*)&_sin, sizeof(_sin)) == SOCKET_ERROR) {
		cout << "bind addr error!" << endl;
	}
	else {
		cout << "bind addr success!" << endl;
	}

	// 3.listen
	if (SOCKET_ERROR == listen(_sock, 5)) {
		cout << "listen port error!" << endl;
	}
	else {
		cout << "listen port success!" << endl;
	}

	// 4.accept
	sockaddr_in clientAddr = {};
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET _cSock = INVALID_SOCKET;

	_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
	if (_cSock == INVALID_SOCKET) {
		cout << "accept error!" << endl;
	}

	cout << "new client join!" << inet_ntoa(clientAddr.sin_addr) << endl;
	while (true) {
		// 5.接收客户端的请求数据
		dataHeader head;
		int nLen = recv(_cSock, (char*)&head, sizeof(head), 0);
		if (nLen <= 0)
		{
			cout << "client close" << endl;
			break;
		}
		switch (head.cmd)
		{
		case LOG_IN:
			{
				login data;
				nLen = recv(_cSock, (char*)&data + sizeof(head), sizeof(data) - sizeof(head), 0);
				if (nLen <= 0){
					cout << "client close" << endl;
					break;
				}
				cout << "cmd:" << data.cmd << endl;
				cout << "len:" << data.dataLen << endl;
				cout << "user_name:" << data.user_name << endl;
				cout << "passwd:" << data.passwd << endl;
				response rsp;
				send(_cSock, (char*)&rsp, sizeof(rsp), 0);
			}
			break;
		case LOG_OUT:
			break;
		default:
			break;
		}
	}

	// 6.close socket
	closesocket(_sock);
	// 清理socket
	WSACleanup();
	cout << "exit" << endl;
	getchar();
	return 0;
}