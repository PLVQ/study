#include "server_message.h"

std::vector<SOCKET> g_clients;
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
		std::cout << "bind addr error!" << std::endl;
	}
	else {
		std::cout << "bind addr success!" << std::endl;
	}

	// 3.listen
	if (SOCKET_ERROR == listen(_sock, 5)) {
		std::cout << "listen port error!" << std::endl;
	}
	else {
		std::cout << "listen port success!" << std::endl;
	}

	while (true)
	{
		fd_set fdRead;
		fd_set fdWrite;
		fd_set fdExp;

		FD_ZERO(&fdRead);
		FD_ZERO(&fdWrite);
		FD_ZERO(&fdExp);

		FD_SET(_sock, &fdRead);
		FD_SET(_sock, &fdWrite);
		FD_SET(_sock, &fdExp);

		for(int i = g_clients.size() - 1; i >= 0; --i)
		{
			// std::cout << g_clients[i] << std::endl;
			FD_SET(g_clients[i], &fdRead);
		}
		timeval tTime = {0, 0};
		int ret = select(_sock + 1, &fdRead, &fdWrite, &fdExp, &tTime);
		if (ret < 0)
		{
			std::cout << "select exit!" << std::endl;
			break;
		}

		// for(int i = g_clients.size() - 1; i >= 0; --i)
		// {
		// 	send_client(g_clients[i]);
		// }

		if (FD_ISSET(_sock, &fdRead))
		{
			FD_CLR(_sock, &fdRead);
			// 4.accept
			sockaddr_in clientAddr = {};
			int nAddrLen = sizeof(sockaddr_in);
			SOCKET _cSock = INVALID_SOCKET;

			_cSock = accept(_sock, (sockaddr *)&clientAddr, &nAddrLen);
			if (_cSock == INVALID_SOCKET)
			{
				std::cout << "accept error!" << std::endl;
			}
			g_clients.push_back(_cSock);
			std::cout << "new client join!" << inet_ntoa(clientAddr.sin_addr) << std::endl;
		}

		for(int i = g_clients.size() - 1; i >= 0; --i)
		{
			if (FD_ISSET(g_clients[i], &fdRead))
			{
				if (-1 == recv_client(g_clients[i]))
				{
					auto iter = std::find(g_clients.begin(), g_clients.end(), g_clients[i]);

					if(iter != g_clients.end())
					{
						g_clients.erase(iter);
					}
				}
			}
		}
	}

	// 6.close socket
	closesocket(_sock);
	// 清理socket
	WSACleanup();
	std::cout << "exit" << std::endl;
	getchar();
	return 0;
}

int recv_client(SOCKET c_sock)
{
	// 5.接收客户端的请求数据
	dataHeader head;
	int nLen = recv(c_sock, (char *)&head, sizeof(head), 0);
	if (nLen <= 0)
	{
		std::cout << "client close" << std::endl;
		return -1;
	}
	switch (head.cmd)
	{
	case LOG_IN:
	{
		login data;
		nLen = recv(c_sock, (char *)&data + sizeof(head), sizeof(data) - sizeof(head), 0);
		if (nLen <= 0)
		{
			std::cout << "client close" << std::endl;
			break;
		}
		std::cout << "cmd:" << data.cmd << std::endl;
		std::cout << "len:" << data.dataLen << std::endl;
		std::cout << "user_name:" << data.user_name << std::endl;
		std::cout << "passwd:" << data.passwd << std::endl;
		response rsp;
		strcpy(rsp.text, "login success!");
		send(c_sock, (char *)&rsp, sizeof(rsp), 0);
	}
	break;
	case LOG_OUT:
		break;
	default:
		break;
	}
	return 0;
}

int send_client(SOCKET c_sock)
{
	response rsp;
	// strcpy(rsp.text, "hello client");
	send(c_sock, (char*)&rsp, sizeof(rsp), 0);
	Sleep(1000);
	return 0;
}