#pragma once
#include <mysql/mysql.h>
#include <string>
using namespace std;

class M_sql{
private:
    //数据库连接
    MYSQL* link;
    MYSQL_ROW row;
    MYSQL_FIELD* field;
    MYSQL_RES* result;

public:
    M_sql(MYSQL* link){
        this->link = link;
    }
    ~M_sql() = default;

    bool set_cookie(const string& cookie, const string& id);

    //查找对应cookie是否在数据库中,如果在数据库中，则返回true并将用户名写入id变量中；如果不在，返回false，id变量不管
    bool find_cookie(const string& cookie, string& id);

    //插入新用户，如果成功返回true，失败返回false
    bool new_user(const string& Uname, const string& passwd, const string& Age, const string& Sex, const string& is_company);

    int confirm_user(const string& user_id, const string& passwd);
    
    bool find_user(const string& name, string& Ped ,string& Age, string& sex, string& is_company);//显示用户的所有信息

    bool new_post(const string& head, const string& author, const string& content);//插入帖子

    bool show_post(string& ret);//显示所有帖子
};
