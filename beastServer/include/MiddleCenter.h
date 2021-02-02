#pragma once



#include "listener.h"
#include "SQL.h"
#include "publicMutex.h"

struct MiddleCenter
{
	MiddleCenter(shared_ptr<io_context> ioc);

	void setHTTPServer(const string & tcpAddress, const string & doc_root);






private:
	shared_ptr<io_context>m_ioc;

	unique_ptr<listener>m_listener;

	bool m_hasSetListener{false};

	shared_ptr<SQL>m_sql;
	
	shared_ptr<publicMutex>m_sqlMutex{ new publicMutex() };

};