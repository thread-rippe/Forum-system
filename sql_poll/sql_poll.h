#include "../lock/lock.h"
#include <mysql/mysql.h>
#include <queue>
using namespace std;

class sql_poll{
private:
    //基础数据
    int max_link_num;

    //空闲队列
    queue<MYSQL*> list;
    
    //线程池模型
    sbuf<MYSQL*> m_lock;

    //数据库数据
    string host = "192.168.25.252";
    string user = "root";
    string passwd = "123456";
    string database = "test";
public:
    sql_poll(int max_num = 8);
    ~sql_poll();

    //单例模式
    static sql_poll* GetSingle(int max);

private:
    MYSQL* get_a_link();
    void release_a_link(MYSQL* link);
}
