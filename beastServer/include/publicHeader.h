#ifndef _PUBLIC_HEADER_H_
#define _PUBLIC_HEADER_H_

#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/predef.h>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/thread.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>



#include <iostream>
#include <string>
#include<memory>
#include<algorithm>
#include<iterator>
#include<functional>
#include<limits>
#include<chrono>
#include<vector>
#include<map>
#include<sstream>
#include<mutex>


using boost::thread_group;
using boost::asio::io_context;
using boost::system::error_code;
using boost::asio::steady_timer;


// using namespace std::chrono_literals;
using std::shared_ptr;
using std::unique_ptr;
using std::cout;
using std::string;
using std::find;
using std::distance;
using std::logical_and;
using std::greater_equal;
using std::less_equal;
using std::placeholders::_1;
using std::stoi;
using std::numeric_limits;
using std::vector;
using std::pair;
using std::stringstream;
using std::make_pair;
using std::mutex;
using std::lock_guard;
using std::reference_wrapper;

static vector<string>interface { "/test/InsertDATA", "/test/getAll","test/student/add" };
static string insertData{ "insertData=" };

static void praseBody(const string &source, const vector<string> &vecFindStr, vector<reference_wrapper<string>> &vecDisStr, bool &success)
{
	if (!source.empty() && !vecFindStr.empty() && !vecDisStr.empty() && vecFindStr.size() == vecDisStr.size())
	{
		vector<string>::const_iterator vecFindStrBegin{ vecFindStr.begin() }, vecFindStrEnd{ vecFindStr.end() };
		vector<reference_wrapper<string>>::iterator vecDisStrBegin{ vecDisStr.begin() }, vecDisStrEnd{ vecDisStr.end() };
		string::const_iterator iterBegin{ source.begin() }, iterEnd{ iterBegin }, iterLast{ source.end() };

		do
		{
			iterBegin = search(iterBegin, iterLast, vecFindStrBegin->begin(), vecFindStrBegin->end());
			if (iterBegin == iterLast)
				break;

			if (distance(vecFindStrBegin, vecFindStrEnd) == 1)
			{
				vecDisStrBegin->get().assign(iterBegin + vecFindStrBegin->size(), iterLast);
				iterBegin = iterLast;
			}
			else
			{
				iterEnd = find(iterBegin + vecFindStrBegin->size(), iterLast, '&');
				if (iterEnd == iterLast)
					break;

				vecDisStrBegin->get().assign(iterBegin + vecFindStrBegin->size(), iterEnd);
				iterBegin = iterEnd;
			}

			++vecFindStrBegin;
			++vecDisStrBegin;

		} while (iterBegin != iterLast);
		success = vecFindStrBegin == vecFindStrEnd;
	}
}

#endif