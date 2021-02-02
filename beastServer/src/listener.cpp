#include "listener.h"
#include "pch.h"

listener::listener(shared_ptr<io_context> ioc, const string & tcpAddress, const string & doc_root ,
	shared_ptr<SQL> sql, shared_ptr<publicMutex> sqlMutex):m_ioc(ioc)
{
	m_doc_root.reset(new string const(doc_root));
	m_tcpAddress = tcpAddress;
	m_sql = sql;
	m_sqlMutex = sqlMutex;
	cout << "start listener\n";
	resetEndpoint();
}


void listener::resetEndpoint()
{
	if (!m_tcpAddress.empty())
	{
		auto iter = find(m_tcpAddress.begin(), m_tcpAddress.end(), ':');
		boost::asio::ip::address ad;
		if (iter != m_tcpAddress.end())
		{
			error_code err;
			ad= boost::asio::ip::address_v4();
			if (err)
			{
				cout << err.value() << " " << err.message() << '\n';
				resetEndpoint();
			}
			else
			{
				if (distance(iter, m_tcpAddress.end()) > 1)
				{
					string port{ iter + 1,m_tcpAddress.end() };
					if (all_of(port.begin(), port.end(), bind(logical_and<bool>(), bind(greater_equal<bool>(), std::placeholders::_1, '0'), bind(less_equal<bool>(), std::placeholders::_1, '9'))))
					{
						m_endpoint.reset(new boost::asio::ip::tcp::endpoint(ad,stoi(port)));
						resetAcceptor();
					}
				}
			}
		}
	}
}


void listener::resetAcceptor()
{
	m_acceptor.reset(new boost::asio::ip::tcp::acceptor(*m_ioc));
	openAcceptor();
}


void listener::openAcceptor()
{
	error_code err;
	m_acceptor->open(m_endpoint->protocol(), err);
	if (err)
	{
		cout << err.value() << " " << err.message() << '\n';
		resetEndpoint();
	}
	else
	{
		bindAcceptor();
	}
}


void listener::bindAcceptor()
{
	error_code err;
	m_acceptor->bind(*m_endpoint,err);
	if (err)
	{
		cout << err.value() << " " << err.message() << '\n';
		resetEndpoint();
	}
	else
	{
		listenAcceptor();
	}
}


void listener::listenAcceptor()
{
	error_code err;
	m_acceptor->listen(numeric_limits<int>::max(), err);
	if (err)
	{
		cout << err.value() << " " << err.message() << '\n';
		resetEndpoint();
	}
	else
	{
		startAccept();
	}
}


void listener::startAccept()
{
	shared_ptr<boost::asio::ip::tcp::socket>m_sock{ new boost::asio::ip::tcp::socket(*m_ioc) };
	m_acceptor->async_accept(*m_sock, [this, m_sock](const boost::system::error_code &err)
	{
		handleStartAccept(err, m_sock);
	});
}



void listener::handleStartAccept(const boost::system::error_code &err , shared_ptr<boost::asio::ip::tcp::socket> sock)
{
	if (err)
	{
		cout << err.value() << " " << err.message() << '\n';
	}
	else
	{
		new Session(m_ioc ,sock, m_doc_root , m_sql ,m_sqlMutex);
		startAccept();
	}
}
