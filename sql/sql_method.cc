#include "sql_method.h"
bool M_sql::find_cookie(const string& cookie, string& id)
{
	string sql = "select cok from cookie where cok ="+cookie;
	mysql_query(&link, sql.c_str());
	result = mysql_store_result(&link);
	if (row = mysql_fetch_row(result))
	{
		id = row[1];
		return true;
	}
	else 
	{
		return false;
	}
}

bool M_sql::new_user(const string& user_id, const string& passwd)
{
		string sql = "insert into person values ( " + user_id + "," + Uname + "," + passwd + "," + Age +  "," + Sex +")";
		int ret = mysql_query(&link, sql.c_str());
		if (ret < 0)
		{
			return false;
		}
		else
		{
			return true;
		}
}

bool M_sql::confirm_user(const stirng& user_id, const string& passwd)
{
	string sql = "select * from person where user = "+user_id;
	mysql_query(&link, sql.c_str())
	result = mysql_store_result(&link);
	if (row = mysql_fetch_row(result))
	{
		if (row[0] == user_id && row[2] == passwd)
		{
			return true;
		}
		else
			return false;
	}
	else
	{
		return false;
	}
}

void M_sql::set_cookie(const string& cookie, const string& name)
{
	string sql = "insert into cookie values ( "+cookie+","+name+")";
	int ret = mysql_query(&link, sql.c_str());
	if (ret < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}