#pragma once

#include "PublicHeader.h"
#include <mysql.h> 
#include<cstdio>


struct SQL
{
	SQL() = default;

	bool ConnectDatabase();

	void FreeConnect();

	bool Mymysql_real_query(const string &source);

	bool MysqlResult(vector<vector<pair<string, string>>>& resultVec);

	~SQL();




private:

	string temp;

	MYSQL mysql;        //

	MYSQL_FIELD *fd;    //字段列数组

	

	MYSQL_RES *res;     //行的一个查询结果集

	MYSQL_ROW column;   //数据行的列

	
	bool hasConnect{ false };

	stringstream execSStr;

	string exec;



	string host{ "127.0.0.1" };
	string user{ "root" };
	string passwd{ "pengjiang22" };
	string db{ "test" };
	unsigned int port = 3306;
	const char *unix_socket{};
	unsigned long client_flag{};

	bool freeMysql{ false };
};