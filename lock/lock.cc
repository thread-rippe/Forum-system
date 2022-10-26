#include "lock.h"

template< typename T>
void sbuf<T>::insert(T* conned){
    unique_lock<mutex> locker(mtx);
    cnd.wait(locker, [this](){ return (tail + 1) % (maxlen + 1) != front; });
    buf[tail] = conned;
    ++tail;
    tail %= (maxlen + 1);
    cnd.notify_one();
}

template< typename T>
T* sbuf<T>::get(){
    unique_lock<mutex> locker(mtx);
    cnd.wait(locker, [this](){ return tail != front; });
    auto ret = buf[front];
    ++front;
    front %= (maxlen + 1);
    cnd.notify_one();
    return ret;
}
