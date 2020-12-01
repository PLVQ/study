#include "easy_tcp_client.hpp"
void cin_cmd(EasyTcpClient *client);
int main()
{
	EasyTcpClient client;
	if(!client.initSocket())
	{
		return 0;
	}	

	if(!client.Connect((char*)"192.168.1.84", 8888))
	{
		return 0;
	}
	// std::thread t1(cin_cmd, &client);
	login request;
	strcpy(request.user_name, "pengjiang");
	strcpy(request.passwd, "123456");
	while(client.isRun())
	{
		client.sendData(&request);
		client.onRun();
	}
	// t1.join();
	client.Close();
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