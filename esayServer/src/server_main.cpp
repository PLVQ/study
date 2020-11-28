#include "server_message.h"

std::vector<SOCKET> g_clients;
int main()
{
#ifdef _WIN32
	// 启动socket
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
#endif
	
	// 1.建立一个socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 2.bind
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(8888);
#ifdef _WIN32
	_sin.sin_addr.S_un.S_addr = INADDR_ANY; // inet_addr("127.0.0.1");
#else
	_sin.sin_addr.s_addr = inet_addr("0.0.0.0");
#endif
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

		SOCKET max_sock = _sock;

		for(int i = g_clients.size() - 1; i >= 0; --i)
		{
			FD_SET(g_clients[i], &fdRead);
			if (max_sock < g_clients[i])
			{
				max_sock = g_clients[i];
			}
		}
		timeval tTime = {0, 0};
		int ret = select(max_sock + 1, &fdRead, &fdWrite, &fdExp, &tTime);
		if (ret < 0)
		{
			std::cout << "select exit!" << std::endl;
			break;
		}

		if (FD_ISSET(_sock, &fdRead))
		{
			FD_CLR(_sock, &fdRead);
			// 4.accept
			sockaddr_in clientAddr = {};
			int nAddrLen = sizeof(sockaddr_in);
			SOCKET _cSock = INVALID_SOCKET;
#ifdef _WIN32
			_cSock = accept(_sock, (sockaddr *)&clientAddr, &nAddrLen);
#else
			_cSock = accept(_sock, (sockaddr *)&clientAddr, (socklen_t *)&nAddrLen);
#endif

			if (_cSock == INVALID_SOCKET)
			{
				std::cout << "accept error!" << std::endl;
			}
			else
			{
				newUserJoin rsp;
				rsp.m_socket = (int)_cSock;
				for(int i = g_clients.size() - 1; i >= 0; --i)
				{
					send(g_clients[i], (char*)&rsp, rsp.dataLen, 0);
				}
				g_clients.push_back(_cSock);
				std::cout << "new client join!" << inet_ntoa(clientAddr.sin_addr) << std::endl;
			}
		}

		for(int i = g_clients.size() - 1; i >= 0; --i)
		{
			if (FD_ISSET(g_clients[i], &fdRead))
			{
				std::cout << "socket " << g_clients[i] << " message!" << std::endl;
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
#ifdef _WIN32
	// 6.close socket
	for (int i = g_clients.size() - 1; i >= 0; --i)
	{
		closesocket(g_clients[i]);
	}
	// 清理socket
	WSACleanup();
#else
	for (int i = g_clients.size() - 1; i >= 0 ; --i)
	{
		close(_sock);
	}
#endif
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
		loginResponse rsp;
		strcpy(rsp.user_name, data.user_name);
		send(c_sock, (char *)&rsp, sizeof(rsp), 0);
	}
	break;
	case LOG_OUT:
	{
		logOut data;
		nLen = recv(c_sock, (char *)&data + sizeof(head), sizeof(data) - sizeof(head), 0);
		if (nLen <= 0)
		{
			std::cout << "client close" << std::endl;
			break;
		}
		std::cout << "cmd:" << data.cmd << std::endl;
		std::cout << "len:" << data.dataLen << std::endl;
		std::cout << "user_name:" << data.user_name << std::endl;
		logOutResponse rsp;
		strcpy(rsp.user_name, data.user_name);
		send(c_sock, (char *)&rsp, sizeof(rsp), 0);
	}
	break;
	default:
		break;
	}
	return 0;
}