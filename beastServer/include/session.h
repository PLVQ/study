#pragma once


#include "publicHeader.h"
#include "SQL.h"
#include "publicMutex.h"


struct Session 
{
	// Take ownership of the stream
	Session(shared_ptr<io_context> ioc , shared_ptr<boost::asio::ip::tcp::socket> sock,
		shared_ptr<std::string const> const& doc_root , shared_ptr<SQL> sql, shared_ptr<publicMutex> sqlMutex);
	
	void run();

	void do_read();

	void fail(boost::beast::error_code ec, char const* what);

	void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);

	
	void on_write(boost::beast::error_code ec, std::size_t bytes_transferred);

    void do_close();

	string path_cat(boost::beast::string_view base, boost::beast::string_view path);

	boost::beast::string_view mime_type(boost::beast::string_view path);


	void bad_request(boost::beast::string_view why);

	void not_found(boost::beast::string_view target);

	void server_error(boost::beast::string_view what);

	void make_head(const uint64_t size);

	void make_response(const string &str);

	void make_get(boost::beast::http::file_body::value_type &body, const uint64_t size);

	void handle_request();

	void startCheckTime();

	void handleInterface();

	void post();

	void put();

	void deleted();

	void get();

	void testInsertDATA();

	void testGetAll();

	void accountAdd();
	
	void accountGet();
	
	void accountUpdate();

	void accountDelete();

private:
	shared_ptr<io_context> m_ioc;
	shared_ptr<boost::asio::ip::tcp::socket>m_socket;
	unique_ptr<steady_timer> m_timer;
	boost::beast::flat_buffer m_buffer;
	shared_ptr<std::string const> m_doc_root;
	

	boost::beast::http::request<boost::beast::http::string_body> m_req;

	boost::beast::http::response<boost::beast::http::string_body> m_res_string_body;
	boost::beast::http::response<boost::beast::http::empty_body> m_res_empty_body;
	boost::beast::http::response<boost::beast::http::file_body> m_res_file_body;

	std::shared_ptr<void> m_res;

	shared_ptr<SQL>m_sql;

	shared_ptr<publicMutex>m_sqlMutex;

	vector<string> vecFindStr;
	vector<reference_wrapper<string>> vecDisStr;
};
