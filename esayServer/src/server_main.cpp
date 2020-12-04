#include "easy_tcp_server.hpp"

int main()
{
	EasyTcpServer server;
	if (!server.initSocket())
	{
		return 0;
	}
	if (!server.Bind((char *)"127.0.0.1", 8888))
	{
		return 0;
	}
	if (!server.Listen())
	{
		return 0;
	}
	while (server.isRun())
	{
		server.onRun();
	}
	server.Close();
	std::cout << "exit" << std::endl;
	getchar();
	return 0;
}