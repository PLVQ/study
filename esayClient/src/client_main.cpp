#include "client_message.h"

int main()
{
	// 启动socket
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);

	// 1.建立socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == _sock) {
		std::cout << "socket error!" << std::endl;
	}
	else {
		std::cout << "socket success!" << std::endl;
	}

	// 2.connect server
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(8888);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int ret = connect(_sock, (sockaddr*)&_sin, sizeof(_sin));
	if (INVALID_SOCKET == ret) {
		std::cout << "connect error!" << std::endl;
	}
	else {
		std::cout << "connect success!" << std::endl;
	}
	while (true)
	{
		// 3.输入命令
		int cmd;
		std::cin >> cmd;
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
		// response rsp;
		// int nLen = recv(_sock, (char*)&rsp, sizeof(rsp), 0);
		// if (nLen > 0) {
		// 	std::cout << "cmd:" << rsp.cmd << std::endl;
		// 	std::cout << "dataLen:" << rsp.dataLen << std::endl;
		// 	std::cout << "ret:" << rsp.ret << std::endl;
		// }
	}
	// 4.关闭socket
	closesocket(_sock);
	// 清理socket
	WSACleanup();
	getchar();
	return 0;
}