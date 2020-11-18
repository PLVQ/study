#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <WinSock2.h>

#include "message.h"

using namespace std;

int main()
{
	// ����socket
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);

	// 1.����socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == _sock) {
		cout << "socket error!" << endl;
	}
	else {
		cout << "socket success!" << endl;
	}

	// 2.connect server
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(8888);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int ret = connect(_sock, (sockaddr*)&_sin, sizeof(_sin));
	if (INVALID_SOCKET == ret) {
		cout << "connect error!" << endl;
	}
	else {
		cout << "connect success!" << endl;
	}
	while (true)
	{
		// 3.��������
		int cmd;
		cin >> cmd;
		login data = {"pengjiang", "123456"};
		dataHeader head;
		head.cmd = cmd;
		head.dataLen = sizeof(data);
		switch (cmd)
		{
		case LOG_IN:
			send(_sock, (char*)&head, sizeof(head), 0);
			send(_sock, (char*)&data, sizeof(data), 0);
			break;
		default:
			break;
		}
		// char cmdBuf[128] = {};
		// cin >> cmdBuf;
		// if (0 == strcmp(cmdBuf, "exit"))
		// {
		// 	cout << "exit" << endl;
		// 	break;
		// }
		// else
		// {
		// 	// ��������
		// 	send(_sock, cmdBuf, strlen(cmdBuf) + 1, 0);
		// }
		// 3.����server data
		response rsp;
		int nLen = recv(_sock, (char*)&rsp, sizeof(rsp), 0);
		if (nLen > 0) {
			cout << rsp.ret << endl;
		}
	}
	// 4.�ر�socket
	closesocket(_sock);
	// ����socket
	WSACleanup();
	getchar();
	return 0;
}