#include "client_message.h"

int main()
{
	// ����socket
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);

	// 1.����socket
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
		fd_set fdRead;

		FD_ZERO(&fdRead);
		FD_SET(_sock, &fdRead);

		int ret = select(_sock + 1, &fdRead, nullptr, nullptr, nullptr);
		if (ret < 0)
		{
			std::cout << "select exit!" << std::endl;
			break;
		}
		if (FD_ISSET(_sock, &fdRead))
		{
			FD_CLR(_sock, &fdRead);
			if (-1 == recv_server(_sock))
			{
				std::cout << "select exit!" << std::endl;
				break;
			}
		}
		send_server(_sock);
		Sleep(1000);
	}
	// 4.�ر�socket
	closesocket(_sock);
	// ����socket
	WSACleanup();
	getchar();
	return 0;
}

int send_server(SOCKET c_sock)
{

	login data;
	strcpy(data.user_name, "pengjiang");
	strcpy(data.passwd, "123456");
	send(c_sock, (char*)&data, sizeof(data), 0);

	return 0;
}

int recv_server(SOCKET c_sock)
{
	dataHeader head;
	int nLen = recv(c_sock, (char *)&head, sizeof(head), 0);
	if (nLen <= 0)
	{
		std::cout << "client close" << std::endl;
		return -1;
	}
	response rsp;
	nLen = recv(c_sock, (char *)&rsp + sizeof(head), sizeof(rsp) - sizeof(head), 0);
	if (nLen <= 0)
	{
		std::cout << "client close" << std::endl;
		return -1;
	}
	std::cout << "len:" << rsp.dataLen << std::endl;
	std::cout << "cmd:" << rsp.cmd << std::endl;
	std::cout << "len:" << rsp.ret << std::endl;
	std::cout << "text:" << rsp.text << std::endl;

	return 0;
}