#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <exception>
#include "../lock/lock.h"
using namespace std;

template <typename T>
class threadpoll
{
public:
    threadpoll(int thread_number = 8);
    ~threadpoll();
    bool append(T& request);
    
private:
    static void* worker(void* arg);     //供多线程调用的静态函数
    void run();

    sbuf<T> lock;                           //在lock文件中实现的队列锁。
    int m_thread_number;//最大进程数
    bool m_stop;                        //是否停止
};

template< typename T >
threadpoll< T >:: threadpoll(int thread_number):
    m_thread_number(thread_number), lock(500), m_stop(false){
        //错误处理
        if(( thread_number <= 0 )){
            throw std::exception();
        }

        //创建线程工作在work上
        for(int i = 0; i < thread_number; ++i){
            thread temp(worker, this);
            temp.detach();
        }
    }

template< typename T >
threadpoll< T >::~threadpoll(){
    m_stop = true;
}

template< typename T >
bool threadpool< T >:: append(T* request){
    lock.insert(request);
}

template< Typename T>
void* threadpool< T >:: worker(void* arg){
    threadpool* pool = (threadpool*)arg;
    pool->run();
    return pool;
}

template< Typename T>
void threadpool< T >:: run(){
    while(!m_stop){
        auto request = lock.get();
        request->process();
    }
}
