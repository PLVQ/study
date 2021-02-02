// BeastHTTP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include "Server.h"
#include "pch.h"

int main()
{
	try
	{
		Server s1;
	
		s1.setHTTPServer("127.0.0.1:8080", "D:/testHTTP");

		
		s1.run();
	}
	catch (const boost::system::system_error &err)
	{
		cout << err.code() << " " << err.what()<<'\n';
		return err.code().value();
	}

	return 0;
}




