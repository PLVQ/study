#include "SQL.h"

bool SQL::ConnectDatabase()
{
	if (!hasConnect)
	{
		if (mysql_init(&mysql))
		{

			if (mysql_real_connect(&mysql, host.data(), user.data(), passwd.data(), db.data(), port, unix_socket, CLIENT_MULTI_STATEMENTS))
			{
				mysql_set_character_set(&mysql, "utf8");
				return true;
			}
			else
			{
				
				hasConnect = false;
				printf("Error connecting to database:%s\n", mysql_error(&mysql));
				return false;
			}
		}
	}

	return false;
}

void SQL::FreeConnect()
{
	if (hasConnect)
	{
		mysql_close(&mysql);
	}
}

bool SQL::Mymysql_real_query(const string & source)
{
	if (!source.empty())
	{
		if (freeMysql)
		{
			while(res = mysql_store_result(&mysql))
				mysql_free_result(res);
			freeMysql = false;
		}

		if (!mysql_real_query(&mysql, &source[0], source.size()))
		{
			freeMysql = true;
			return true;
		}


		printf("Query failed (%s)\n", mysql_error(&mysql));
		cout << source << '\n';
	}

	return false;
}

bool SQL::MysqlResult(vector<vector<pair<string, string>>>& resultVec)
{

	int num;
	vector<pair<string, string>> result;
	resultVec.clear();
	if (!(res = mysql_store_result(&mysql)))    //���sql�������󷵻صĽ����
	{
		printf("Couldn't get result from %s\n", mysql_error(&mysql));
		return false;
	}
	freeMysql = false;

	//��ӡ��������
	if (!mysql_affected_rows(&mysql))
		return false;

	if (!(num = mysql_field_count(&mysql)))
		return false;

	//��ȡ�ֶε���Ϣ

	for (int i = 0; i < num; i++)  
	{
		result.emplace_back(make_pair(mysql_fetch_field(res)->name, ""));
	}


	while (column = mysql_fetch_row(res))   //����֪�ֶ���������£���ȡ����ӡ��һ��
	{
		for (int i = 0; i != num; ++i)
		{
			if (column[i])
				result[i].second = column[i];
			else 
				result[i].second = "";
		}
		resultVec.emplace_back(result);

	}

	mysql_free_result(res);

	return true;
}

SQL::~SQL()
{
	FreeConnect();
}


























