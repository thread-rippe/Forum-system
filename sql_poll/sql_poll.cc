#include "sql_poll.h"

sql_poll* sql_poll::GetSingle(int max){
    static sql_poll only_poll(max);
    return only_poll;
}

MYSQL* get_a_link(){
    return m_lock.get();
}

void sql_poll::release_a_link(MYSQL* link){
    m_lock.insert(link);
}

//创建连接池
sql_poll::sql_poll(int max): max_link_num(max), m_lock(max){
    for(int i = 0; i < max; ++i){
        MYSQL* temp;
        if(mysql_init(&mysql) == nullptr){
            cout << "数据库初始化错误" << endl;
            assert(0);
        }
        if(!mysql_real_connect(&temp, host.c_str(), user.c_str(), passwd.c_str(), database.c_str(), 0, nullptr, 0)){
            cout << "数据库连接错误" << endl;
            assert(0);
        }
        m_lock.insert(temp);
    }
}

sql_poll::~sql_poll(){
    for(int i = 0; i < max_link_num; ++i){
        auto temp = m_lock.get();
        mysql_close(temp);
    }
}
