#include "MiddleCenter.h"
#include "pch.h"

MiddleCenter::MiddleCenter(shared_ptr<io_context> ioc):m_ioc(ioc)
{
	m_sql.reset(new SQL());
	if (m_sql->ConnectDatabase())
	{
		cout << "mysql connect success\n";
	}
}


void MiddleCenter::setHTTPServer(const string & tcpAddress, const string & doc_root)
{
	if (!m_hasSetListener)
	{
		m_hasSetListener = true;
		m_listener.reset(new listener(m_ioc, tcpAddress, doc_root , m_sql , m_sqlMutex));

	}
}
