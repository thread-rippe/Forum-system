#include "sql_method.h"
bool M_sql::find_cookie(const string& cookie, string& id)
{
	string sql = "select Uid from cookie where cok = '"+cookie + "'";
    cout << "查询前" << endl;
    if(!mysql_query(link, sql.c_str())){
        cout << "查询成功" << endl;
    }else{
        cout << "查询失败" << endl;
        return false;
    }
    if((result = mysql_store_result(link))){
        cout << "得到结果集" << endl;
    }else{
        cout << "没有得到结果集" << endl;
        return false;
    }
    cout << "查询后" << endl;
	if (row = mysql_fetch_row(result))
	{
        cout << "成功" << endl;
		id = row[0];
		return true;
	}
	else 
	{
        cout << "失败" << endl;
		return false;
	}
}

bool M_sql::new_user(const string& Uname, const string& passwd, const string& Age, const string& Sex)
{
	    //string sql = "insert into person(uname, ped, age, sex) values("+Uname+","+ passwd+"," + Age + "," + Sex + ")";
		string sql = "insert into user(uname, ped, age, sex) values ('" + Uname + "','" + passwd + "','" + Age +  "','" + Sex +"')";
		int ret = mysql_query(link, sql.c_str());
        cout << "执行插入新用户语句" << endl;
		if (ret < 0)
		{
			return false;
		}
		else
		{
			return true;
		}
}

bool M_sql::confirm_user(const string& user_id, const string& passwd)
{
    cout << user_id << endl;
    string sql = "select Uname, Ped from user where Uname = '"+user_id + "'";
    cout << "登陆前" << endl;
    if(mysql_query(link, sql.c_str())){
        cout << "查询失败" << endl;
        return false;
    }
    if(!(result = mysql_store_result(link))){
        cout << "结果集获取失败";
        return false;
    }
    cout << "登陆后" << endl;
	if (row = mysql_fetch_row(result))
	{
        cout << "确认用户中" << endl;
		if (row[0] == user_id && row[1] == passwd)
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

bool M_sql::set_cookie(const string& cookie, const string& name)
{
	string sql = "insert into cookie(cok, Uid) values ('"+cookie+"','"+name+"')";
    //cout << sql << endl;
	int ret = mysql_query(link, sql.c_str());
    cout << "执行cookie插入语句" << endl;
	if (ret < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
