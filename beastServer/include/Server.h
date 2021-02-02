#pragma once

#include "publicHeader.h"
#include "MiddleCenter.h"



struct Server
{
	Server();

	void setHTTPServer(const string &tcpAddress, const string &doc_root);

	void setThreadNum(const size_t threadNum=0);

	void run();

private:
	shared_ptr<io_context>m_ioc{new io_context() };
	thread_group m_tg;
	io_context::work m_work{ *m_ioc };

	bool m_hasRun{ false };
	bool m_hasSetThreadNum{ false };


	unique_ptr<MiddleCenter>m_MiddleCenter;
};