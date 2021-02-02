#include "Server.h"
#include "pch.h"

Server::Server()
{
	m_MiddleCenter.reset(new MiddleCenter(m_ioc));
}

void Server::setHTTPServer(const string & tcpAddress, const string & doc_root)
{
	m_MiddleCenter->setHTTPServer(tcpAddress, doc_root);
}

void Server::setThreadNum(const size_t threadNum)
{
	if (!m_hasSetThreadNum)
	{
		m_hasSetThreadNum = true;
		size_t num{ threadNum };
		if (!num)
		{
			// num = std::thread::hardware_concurrency();
		}
		if (!num)
			num = 1;
		for (int i = 0; i != num; ++i)
		{
			m_tg.create_thread([this]
			{
				m_ioc->run();
			});
		}
	}

}





void Server::run()
{
	if (!m_hasRun)
	{
		if (!m_hasSetThreadNum)
		{
			setThreadNum();
		}

		m_hasRun = true;
		m_ioc->run();
	}
}





