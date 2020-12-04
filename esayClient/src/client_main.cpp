#include "easy_tcp_client.hpp"
void cin_cmd(EasyTcpClient *client);
int main()
{
	const int cCount = 10;
	EasyTcpClient *clients[cCount];

	for (int i = 0; i < cCount; ++i)
	{
		clients[i] = new EasyTcpClient;
		if (!clients[i]->initSocket())
		{
			return 0;
		}
	}

	for (int i = 0; i < cCount; ++i)
	{
		if (!clients[i]->Connect((char *)"127.0.0.1", 8888))
		{
			return 0;
		}
	}

	login request;
	strcpy(request.user_name, "pengjiang");
	strcpy(request.passwd, "123456");

	while (true)
	{
		for (int i = 0; i < cCount; ++i)
		{
			clients[i]->sendData(&request);
			int ret = clients[i]->onRun();
		}
	}
	for (int i = 0; i < cCount; ++i)
	{
		clients[i]->Close();
	}
	getchar();
	return 0;
}

void cin_cmd(EasyTcpClient *client)
{
	int cmd;
	while (true)
	{
		std::cin >> cmd;
		if (cmd == -1)
		{
			break;
		}
		switch (cmd)
		{
		case LOG_IN:
		{
			login request;
			strcpy(request.user_name, "pengjiang");
			strcpy(request.passwd, "123456");

			int ret = client->sendData(&request);
		}
		break;
		case LOG_OUT:
		{
			logOut request;
			strcpy(request.user_name, "pengjiang");

			int ret = client->sendData(&request);
		}
		break;
		default:
			std::cout << "error cmd!" << std::endl;
			break;
		}
	}
}