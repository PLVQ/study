#include "session.h"
#include "pch.h"

using namespace boost::property_tree;
Session::Session(shared_ptr<io_context> ioc ,shared_ptr<boost::asio::ip::tcp::socket> sock, shared_ptr<std::string const> const & doc_root,
      shared_ptr<SQL> sql, shared_ptr<publicMutex> sqlMutex) :m_doc_root(doc_root)
{
	m_ioc = ioc;
	m_socket = sock;
	m_timer.reset(new steady_timer(*m_ioc));
	m_sql = sql;
	m_sqlMutex = sqlMutex;
	cout << "start session  " << m_socket->remote_endpoint().address().to_string() << " " << m_socket->remote_endpoint().port() << '\n';
	run();
}


void Session::run()
{
	startCheckTime();
	do_read();
}


void Session::do_read()
{
	m_req = {};

	m_timer->cancel();

	boost::beast::http::async_read(*m_socket, m_buffer, m_req, [this](const error_code &err, std::size_t size)
	{
		on_read(err, size);
	});
}


void Session::fail(boost::beast::error_code ec, char const * what)
{
	std::cerr << what << ": " << ec.message() << "\n";
}



void Session::on_read(boost::beast::error_code ec, std::size_t bytes_transferred)
{
	boost::ignore_unused(bytes_transferred);

	if (ec == boost::beast::http::error::end_of_stream)
		return ;

	if (ec)
		return fail(ec, "read");

	handle_request();
}



void Session::on_write(boost::beast::error_code ec, std::size_t bytes_transferred)
{
	boost::ignore_unused(bytes_transferred);

	if (ec)
		return fail(ec, "write");

	// We're done with the response so delete it
	m_res = {};

	// Read another request
	do_read();
}



void Session::do_close()
{
	// Send a TCP shutdown
	boost::beast::error_code ec;
	m_socket->cancel(ec);
	m_timer->expires_from_now(std::chrono::seconds(5));
	m_timer->async_wait([this](const boost::system::error_code &err)
	{
		error_code ec;
		m_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
		m_timer->expires_from_now(std::chrono::seconds(5));
		m_timer->async_wait([this](const boost::system::error_code &err)
		{
			error_code ec;
			m_socket->close(ec);
			m_timer->expires_from_now(std::chrono::seconds(5));
			m_timer->async_wait([this](const boost::system::error_code &err)
			{
				cout << "delete this\n";
				delete this;
			});
		});
	});
}



string Session::path_cat(boost::beast::string_view base, boost::beast::string_view path)
{
	if (base.empty())
		return std::string(path);
	std::string result(base);
#ifdef BOOST_MSVC
	char constexpr path_separator = '\\';
	if (result.back() == path_separator)
		result.resize(result.size() - 1);
	result.append(path.data(), path.size());
	for (auto& c : result)
		if (c == '/')
			c = path_separator;
#else
	char constexpr path_separator = '/';
	if (result.back() == path_separator)
		result.resize(result.size() - 1);
	result.append(path.data(), path.size());
#endif
	return result;
}



boost::beast::string_view Session::mime_type(boost::beast::string_view path)
{
	using boost::beast::iequals;
	auto const ext = [&path]
	{
		auto const pos = path.rfind(".");
		if (pos == boost::beast::string_view::npos)
			return boost::beast::string_view{};
		return path.substr(pos);
	}();
	if (iequals(ext, ".htm"))  return "text/html";
	if (iequals(ext, ".html")) return "text/html";
	if (iequals(ext, ".php"))  return "text/html";
	if (iequals(ext, ".css"))  return "text/css";
	if (iequals(ext, ".txt"))  return "text/plain";
	if (iequals(ext, ".js"))   return "application/javascript";
	if (iequals(ext, ".json")) return "application/json";
	if (iequals(ext, ".xml"))  return "application/xml";
	if (iequals(ext, ".swf"))  return "application/x-shockwave-flash";
	if (iequals(ext, ".flv"))  return "video/x-flv";
	if (iequals(ext, ".png"))  return "image/png";
	if (iequals(ext, ".jpe"))  return "image/jpeg";
	if (iequals(ext, ".jpeg")) return "image/jpeg";
	if (iequals(ext, ".jpg"))  return "image/jpeg";
	if (iequals(ext, ".gif"))  return "image/gif";
	if (iequals(ext, ".bmp"))  return "image/bmp";
	if (iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
	if (iequals(ext, ".tiff")) return "image/tiff";
	if (iequals(ext, ".tif"))  return "image/tiff";
	if (iequals(ext, ".svg"))  return "image/svg+xml";
	if (iequals(ext, ".svgz")) return "image/svg+xml";
	return "application/text";
}


void Session::bad_request(boost::beast::string_view why)
{
	m_res_string_body={ boost::beast::http::status::bad_request, m_req.version() };
	m_res_string_body.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
	m_res_string_body.set(boost::beast::http::field::content_type, "text/html");
	m_res_string_body.keep_alive(m_req.keep_alive());
	m_res_string_body.body() = std::string(why);
	m_res_string_body.prepare_payload();

	boost::beast::http::async_write(*m_socket, m_res_string_body, [this](const error_code &err, std::size_t size)
	{
		on_write(err, size);
	});
}


void Session::not_found(boost::beast::string_view target)
{
	m_res_string_body={ boost::beast::http::status::not_found, m_req.version() };
	m_res_string_body.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
	m_res_string_body.set(boost::beast::http::field::content_type, "text/html");
	m_res_string_body.keep_alive(m_req.keep_alive());
	m_res_string_body.body() = "The resource '" + std::string(target) + "' was not found.";
	m_res_string_body.prepare_payload();

	boost::beast::http::async_write(*m_socket, m_res_string_body, [this](const error_code &err, std::size_t size)
	{
		on_write(err, size);
	});
}


void Session::server_error(boost::beast::string_view what)
{
	m_res_string_body={ boost::beast::http::status::internal_server_error, m_req.version() };
	m_res_string_body.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
	m_res_string_body.set(boost::beast::http::field::content_type, "text/html");
	m_res_string_body.keep_alive(m_req.keep_alive());
	m_res_string_body.body() = "An error occurred: '" + std::string(what) + "'";
	m_res_string_body.prepare_payload();

	boost::beast::http::async_write(*m_socket, m_res_string_body, [this](const error_code &err, std::size_t size)
	{
		on_write(err, size);
	});
}


void Session::make_head(const uint64_t size)
{
	std::string path = path_cat(*m_doc_root, m_req.target());
	m_res_empty_body={ boost::beast::http::status::ok, m_req.version() };
	m_res_empty_body.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
	m_res_empty_body.set(boost::beast::http::field::content_type, mime_type(path));
	m_res_empty_body.content_length(size);
	m_res_empty_body.keep_alive(m_req.keep_alive());

	boost::beast::http::async_write(*m_socket, m_res_empty_body, [this](const error_code &err, std::size_t size)
	{
		on_write(err, size);
	});
}

void Session::make_response(const string & str)
{
	m_res_string_body={ boost::beast::http::status::ok, m_req.version() };
	m_res_string_body.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
	m_res_string_body.keep_alive(m_req.keep_alive());
	m_res_string_body.body() = str;
	m_res_string_body.prepare_payload();

	boost::beast::http::async_write(*m_socket, m_res_string_body, [this](const error_code &err, std::size_t size)
	{
		on_write(err, size);
	});
}


void Session::make_get(boost::beast::http::file_body::value_type &body , const uint64_t size)
{
	std::string path = path_cat(*m_doc_root, m_req.target());
	m_res_file_body={
		std::piecewise_construct,
		std::make_tuple(std::move(body)),
		std::make_tuple(boost::beast::http::status::ok, m_req.version()) };
	m_res_file_body.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
	m_res_file_body.set(boost::beast::http::field::content_type, mime_type(path));
	m_res_file_body.content_length(size);
	m_res_file_body.keep_alive(m_req.keep_alive());

	boost::beast::http::async_write(*m_socket, m_res_file_body, [this](const error_code &err, std::size_t size)
	{
		on_write(err, size);
	});
}



void Session::handle_request()
{

	if (m_req.method() != boost::beast::http::verb::get &&
		m_req.method() != boost::beast::http::verb::head &&
		m_req.method() != boost::beast::http::verb::post)
		return bad_request("Unknown HTTP-method");

	if (m_req.method() == boost::beast::http::verb::post)
	{
		return handleInterface();
	}


	// Request path must be absolute and not contain "..".
	if (m_req.target().empty() ||
		m_req.target()[0] != '/' ||
		m_req.target().find("..") != boost::beast::string_view::npos)
		return bad_request("Illegal request-target");

	// Build the path to the requested file
	std::string path = path_cat(*m_doc_root, m_req.target());
	if (m_req.target().back() == '/')
		path.append("index.html");

	// Attempt to open the file
	boost::beast::error_code ec;
	boost::beast::http::file_body::value_type body;
	body.open(path.c_str(), boost::beast::file_mode::scan, ec);

	// Handle the case where the file doesn't exist
	if (ec == boost::beast::errc::no_such_file_or_directory)
		return not_found(m_req.target());

	// Handle an unknown error
	if (ec)
		return server_error(ec.message());

	// Cache the size since we need it after the move
	auto const size = body.size();

	// Respond to HEAD request
	if (m_req.method() == boost::beast::http::verb::head)
		return make_head(size);

	// Respond to GET request

	return make_get(body, size);
}

void Session::startCheckTime()
{
	m_timer->expires_after(std::chrono::seconds(30));

	m_timer->async_wait([this](const boost::system::error_code &err)
	{
		if (err)
		{
			if (err == boost::asio::error::operation_aborted)
			{
				startCheckTime();
			}
			return;
		}
		else
		{
			do_close();
		}
	});
}


void Session::handleInterface()
{
	std::cout << m_req.body() << std::endl;
	std::cout << m_req.target() << std::endl;
	std::cout << m_req.base().target() << std::endl;
	if (!interface.empty())
	{
		auto vecIter = interface.begin();
		for (; vecIter != interface.end(); ++vecIter)
		{
			if (search(m_req.target().begin(), m_req.target().end(), vecIter->begin(), vecIter->end()) != m_req.target().end())
			{
				break;
			}
		}
		switch (distance(interface.begin(),vecIter))
		{
		case 0:
			cout << "/test/InsertDATA\n";
			testInsertDATA();
			break;
		case 1:
			cout << "/test/getAll\n";
			testGetAll();
			break;
		default:
			cout << "unknown interface\n";
			do_read();
			break;
		}
	}
}

void Session::post()
{
	std::cout << m_req.body() << std::endl;
	std::cout << m_req.target() << std::endl;
	std::cout << m_req.base().target() << std::endl;
	if (!interface.empty())
	{
		auto vecIter = interface.begin();
		for (; vecIter != interface.end(); vecIter++)
		{
			if (search(m_req.target().begin(), m_req.target().end(), vecIter->begin(), vecIter->end()) != m_req.target().end())
			{
				break;
			}
		}
		switch (distance(interface.begin(),vecIter))
		{
		case 0:
			cout << "/test/InsertDATA\n";
			testInsertDATA();
			break;
		case 1:
			cout << "/test/getAll\n";
			testGetAll();
			break;
		case 2:
			cout << "/test/account\n";
			accountAdd();
			break;
		default:
			cout << "unknown interface\n";
			do_read();
			break;
		}
	}
}

void Session::testInsertDATA()
{
	string insertDataStr;
	string result;
	bool success{ false };
	vecFindStr.assign({ insertData });
	vecDisStr.assign({ insertDataStr });
	m_sql->Mymysql_real_query("call pro_student_info_add(\'"+m_req.body()+"\')");
	praseBody(m_req.body(), vecFindStr, vecDisStr, success);
	if (!success || insertDataStr.empty())
	{
		return bad_request("Illegal request");
	}
	else
	{
		
		success = false;
		stringstream sstr;
		do
		{
			sstr << "insert into testTable(data)  select'" << insertDataStr << "' from dual where not exists(select * from testTable where data='" << insertDataStr << "')";
			{
				lock_guard<mutex>l1{ m_sqlMutex->m1 };
				if (!m_sql->Mymysql_real_query(sstr.str()))
					break;
			}

			success = true;
		} while (0);
		if (success)
		{
			result = "success insert data";
		}
		else
		{
			result = "fail to insert data";
		}
		make_response(result);		
	}
}


void Session::testGetAll()
{
	bool success{ false };
	stringstream sstr;
	ptree ptjson, pt1, pt2;
	string result;
	vector<vector<pair<string, string>>> resultVec;
//	ptree p;
//	std::stringstream ss("http://httpbin.org/get?ie=utf-8");
////	boost::property_tree::read_json(ss, p);
//	std::cout << p.get<std::string>("ie") << std::endl;
//	std::cout << m_req.base().target() << std::endl;
	do
	{
		sstr << "select * from testTable ";
		{
			lock_guard<mutex>l1{ m_sqlMutex->m1 };
			if (!m_sql->Mymysql_real_query(sstr.str()))
				break;

			if (!m_sql->MysqlResult(resultVec))
				break;
		}

		success = true;
	} while (0);
	if (success)
	{
		for (auto const &innerVec : resultVec)
		{
			for (auto const &myPair : innerVec)
			{
				pt1.put(myPair.first, myPair.second);
			}
			pt2.push_back(make_pair("", pt1));
			pt1.clear();
		}

		ptjson.put_child("result", pt2);
		sstr.str("");
		sstr.clear();
		write_json(sstr, ptjson);

		result = sstr.str();	
	}
	else
	{
		result = "fail to insert data";
	}
	make_response(result);
}

void Session::accountAdd()
{

}
