#pragma once
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inte.h>
#include <assert.h>
#include <sys/stat.h>
#include <string>
#include <thread>
#include <iostream>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include <mutex>

//为线程池准备的模板参数类
class http_conn{
public:
    //文件的最大长度
    static const int FILENAME_LEN = 200;
    //缓冲区的大小
    static const int READ_BUFFER_SIZE = 2048;
    //写缓冲区的大小
    static const int WRITE_BUFFER_SIZE = 1024;
    
    //主状态机所处的状态
    enum CHECK_STATE { CHECK_STATE_REQUESTLINE = 0,
                        CHECK_STATE_HEADER,
                        CHECK_STATE_CONTENT};
    //处理http请求可能得到的结果
    enum HTTP_CODE {NO_REQUEST, GET_REQUEST, BAD_REQUEST, 
                    NO_RESOURCE, FORBIDDEN_REQUEST, FILE_REQUEST,
                    INTERNAL_ERROR, CLOSED_CONNECTION};
    //行读取状态
    enum LINE_STATUS {LINE_OK = 0, LINE_BAD, LINE_OPEN};

public:
    http_conn(){};
    ~http_coon(){};

public:
    //初始化新接受的连接
    void init(int sockfd, const sockaddr_in& addr);
    //关闭连接
    void close_conn(bool real_close = true);
    //处理请求函数
    void process();
    //非阻塞读函数
    bool read();
    //非阻塞写
    bool write();

private:
    //初始化
    void init();
    //解析http
    HTTP_CODE process_read();
    //填充http应答
    bool process_write(HTTP_CODE ret);

    //下面的这一组函数用来分析，准备考虑将char转换为string
    HTTP_CODE parse_request_line(const string& text);
    HTTP_CODE parse_headers(const string& text);
    HTTP_CODE parse_content(const string& text);
    HTTP_CODE do_request();
    char* get_line() {return m_read_buf + m_start_line};
    LINE_STATUS parse_line();

    //以下用来填充http应答
    void unmap();
    bool add_response(const string& format);
    bool add_content(const string& content);
    bool add_status_line(int status, const string& title);
    bool add_headers(int content_length);
    bool add_content_length(int content_length);
    bool add_linger();
    bool add_blank_link();

public:
    //所有事件都注册在同一个epoll上所以统一即可
    static int m_epollfd;
    //用户总量
    static int m_user_count;

private:
    //socket描述符和对方的地址
    int m_sockfd;
    sockaddr_in m_address;

    //读缓冲区
    char m_read_buf[ READ_BUFFER_SIZE ];
    //下一次读起始
    int m_read_idx;
    //当前正在解析的行起始位置
    int m_checked_idx;
    int m_start_line;
    string m_write_buf;

}
