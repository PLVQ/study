#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
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
		cout << "绑定地址失败!" << endl;
	}
	else {
		cout << "绑定地址成功!" << endl;
	}

	// 3.listen
	if (SOCKET_ERROR == listen(_sock, 5)) {
		cout << "监听端口失败!" << endl;
	}
	else {
		cout << "监听端口成功!" << endl;
	}

	// 4.accept
	sockaddr_in clientAddr = {};
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET _cSock = INVALID_SOCKET;
	char msgBuff[] = "hello, i'm server.";

	_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
	if (_cSock == INVALID_SOCKET) {
		cout << "error" << endl;
	}

	cout << "new client join!" << inet_ntoa(clientAddr.sin_addr) << endl;
	char _recvBuf[128] = {};
	while (true) {
		// 5.接收客户端的请求数据
		int nLen = recv(_cSock, _recvBuf, 128, 0);
		if (nLen <= 0)
		{
			cout << "client close" << endl;
			break;
		}
		cout << "cmd:" << _recvBuf << endl;
		// 6.处理请求
		if (0 == strcmp(_recvBuf, "getName"))
		{
			char msgBuff[] = "pengjiang.";
			send(_cSock, msgBuff, strlen(msgBuff) + 1, 0);
		}
		else if (0 == strcmp(_recvBuf, "getAge"))
		{
			char msgBuff[] = "80.";
			send(_cSock, msgBuff, strlen(msgBuff) + 1, 0);
		}
		else
		{
			char msgBuff[] = "hello, i'm server.";
			send(_cSock, msgBuff, strlen(msgBuff) + 1, 0);
		}
		// 6.send
		// send(_cSock, msgBuff, strlen(msgBuff) + 1, 0);
	}

	// 6.close socket
	closesocket(_sock);
	// 清理socket
	WSACleanup();
	cout << "exit" << endl;
	getchar();
	return 0;
}