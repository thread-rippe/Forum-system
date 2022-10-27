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

    //查找对应cookie是否在数据库中,如果在数据库中，则返回true并将用户名写入id变量中；如果不在，返回false，id变量不管
    bool find_cookie(const string& cookie, string& id);
    //插入新用户，如果成功返回true，失败返回false
    bool new_user(const string& user_id, const string& passwd);
    //确认用户，查找是否有这个用户并且密码正确如果没问题则返回true，有问题返回false
    bool confirm_user(const string& user_id, const string& passwd);
    void set_cookie(const string& cookie, const string& name);
};
