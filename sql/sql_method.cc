#include "sql_method.h"

bool M_sql::set_cookie(const string& cookie, const string& id)
{
	string temp = "";
	if (find_cookie(cookie, temp))
	{
		string sql1 = "update cookie set Uid='" + id + "' where cok = '" + cookie + "' ";
		bool ret = mysql_query(link, sql1.c_str());
		cout << "执行Uid修改语句" << endl;
		if (!ret)
		{
			cout << "修改Uid失败" << endl;
			return false;
		}
		else
		{
			cout << "修改Uid成功" << endl;
			return true;
		}
	}
	else
	{
		string sql = "insert into cookie(cok, Uid) values ('" + cookie + "','" + id + "')";
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
}

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
        mysql_free_result(result);
		return true;
	}
	else 
	{
        cout << "失败" << endl;
        mysql_free_result(result);
		return false;
	}
}

bool M_sql::new_user(const string& Uname, const string& passwd, const string& Age, const string& Sex, const string& is_company)//合并了个人和企业表，个人F，企业T
{
		string sql = "insert into user(uname, ped, age, sex, is_company) values ('" + Uname + "','" + passwd + "','" + Age +  "','" + Sex +"','"+is_company+"')";
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

int M_sql::confirm_user(const string& user_id, const string& passwd)//改变返回返回值，个人1，企业2，错0
{
    //mysql_free_result(result);
    cout << user_id << endl;
    string sql = "select Uname, Ped, iscompany from user where Uname = '"+user_id + "'";
    cout << "登陆前" << endl;
    int k;
    if((k = mysql_query(link, sql.c_str()))){
        cout << "查询失败" << endl;
        cout << k << endl;
        return 0;
    }
    if(!(result = mysql_store_result(link))){
        cout << "结果集获取失败";
        return 0;
    }
    cout << "登陆后" << endl;
	if (row = mysql_fetch_row(result))
	{
        cout << "确认用户中" << endl;
		if (row[0] == user_id && row[1] == passwd)
		{
			cout << "确认完成，区分个人与企业" << endl;
            //return 1;
			//当用户id及密码匹配后，区分个人和企业
			if (row[2] == "T")
			{
                mysql_free_result(result);
				return 2;
			}
			else{
                mysql_free_result(result);
				return 1;
		    }
        }
        mysql_free_result(result);
		return 0;
	}
	else
	{
        mysql_free_result(result);
		return 0;
	}
}

bool M_sql::find_user(const string& name, string& Ped ,string& Age, string& sex, string& is_company)//显示用户的所有信息
{
    //mysql_free_result(result);
	string sql = "select Ped,Age,Sex,iscompany from user where Uname = '" + name + "'";
	cout << "查询用户信息前" << endl;
	if (!mysql_query(link, sql.c_str())) {
		cout << "查询成功" << endl;
	}
	else {
		cout << "查询失败" << endl;
		return false;
	}
	if ((result = mysql_store_result(link))) {
		cout << "得到结果集" << endl;
	}
	else {
		cout << "没有得到结果集" << endl;
		return false;
	}
	cout << "查询用户信息后" << endl;
	if (row = mysql_fetch_row(result))
	{
		cout << "成功" << endl;
		Ped = row[0];
		Age = row[1];
		sex = row[2];
		is_company = row[3];
        mysql_free_result(result);
		return true;
	}
	else
	{
        mysql_free_result(result);
		cout << "失败" << endl;
		return false;
	}
}

bool M_sql::new_post(const string& head, const string& author, const string& content)//插入帖子
{
	string sql = "insert into post(title, author, content) values ('" + head + "','" + author + "','" + content + "')";
	int ret = mysql_query(link, sql.c_str());
	cout << "执行插入新贴子语句" << endl;
	if (ret < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool M_sql::show_post(string& ret)
{
	unsigned int num_fields;
	string sql = "select Author, title, content from post";
	cout << "显示帖子前" << endl;
	if (!mysql_query(link, sql.c_str())) {
		cout << "查询成功" << endl;
	}
	else {
		cout << "查询失败" << endl;
		return false;
	}
	if ((result = mysql_store_result(link))) {
		cout << "得到结果集" << endl;
	}
	else {
		cout << "没有得到结果集" << endl;
        mysql_free_result(result);
		return false;
	}
	cout << "查询后" << endl;
	num_fields = mysql_num_fields(result);
	cout << "成功" << endl;
	ret += "<!DOCTYPE html><html>\r\n";
	ret += "<head><title>帖子</title><meta http-equiv=\"Content-Type\" content=\"textml;charset=utf-8\" /> </head>\r\n";
	ret += "<body>";
	while (row = mysql_fetch_row(result))
	{
		ret += "<p><big>";
		ret += row[1]; 
        ret += "</big>";
		ret += row[0];
        ret += "</p><br/>";
		ret += "<p>";
        ret += row[2];
        ret += "</p><br/>";
	}
    mysql_free_result(result);
	return true;
}
