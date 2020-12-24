#include "server.h"

int main()
{
#ifdef _WIN32
	// windows启动socket
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
#endif
	Server server;
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
	server.Start();
	while (server.isRun())
	{
		server.onRun();
	}
	server.Close();
	std::cout << "exit" << std::endl;
#ifdef _WIN32
	// windows关闭socket
	WSACleanup();
#endif
	getchar();
	return 0;
}