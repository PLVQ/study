#pragma once


#include "publicHeader.h"
#include "session.h"
#include "SQL.h"
#include "publicMutex.h"

struct listener 
{
	listener(shared_ptr<io_context>ioc, const string &tcpAddress, const string &doc_root,
		shared_ptr<SQL> sql,shared_ptr<publicMutex> sqlMutex);

	void resetEndpoint();

	void resetAcceptor();

	void openAcceptor();

	void bindAcceptor();

	void listenAcceptor();

	void startAccept();

	void handleStartAccept(const boost::system::error_code &err, shared_ptr<boost::asio::ip::tcp::socket> sock);


private:
	shared_ptr<io_context> m_ioc;
	unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
	unique_ptr<boost::asio::ip::tcp::endpoint>m_endpoint;
	shared_ptr<std::string const> m_doc_root;
	string m_tcpAddress;

	shared_ptr<SQL>m_sql;

	shared_ptr<publicMutex>m_sqlMutex;
};