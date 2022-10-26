#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>
using namespace std;

//生产者消费者模型的缓冲区实现
//使用循环队列模型外加多线程操作
template< typename T >
class sbuf{
private:
    vector<T*> buf; //保存描述符的
    int maxlen; //最大容量
    int front, tail; //头指针尾指针
    mutex mtx;
    condition_variable cnd; 

    public:
    sbuf(int max): maxlen(max), buf(max + 1), front(0), tail(0) {};
    ~sbuf() = default;

    //明确删除我不需要的函数
    sbuf(const sbuf&) = delete;
    sbuf& operator=(const sbuf&) = delete;

    //向数组中插入删除描述符
    void insert(T* conned);
    T* get();
}; 
