#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <WinSock2.h>

#include "message.h"

using namespace std;

int main()
{
	// 启动socket
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);

	// 1.建立socket
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
		// 3.输入命令
		int cmd;
		cin >> cmd;
		switch (cmd)
		{
		case LOG_IN:
			{
				login data;
				strcpy(data.user_name, "pengjiang");
				strcpy(data.passwd, "123456");
				send(_sock, (char*)&data, sizeof(data), 0);
			}
			break;
		default:
			break;
		}
		response rsp;
		int nLen = recv(_sock, (char*)&rsp, sizeof(rsp), 0);
		if (nLen > 0) {
			cout << "cmd:" << rsp.cmd << endl;
			cout << "dataLen:" << rsp.dataLen << endl;
			cout << "ret:" << rsp.ret << endl;
		}
	}
	// 4.关闭socket
	closesocket(_sock);
	// 清理socket
	WSACleanup();
	getchar();
	return 0;
}