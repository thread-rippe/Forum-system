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
    
    /*主状态机当前所处的状态*/
    CHECK_STATE m_check_state;
    /*请求方法*/
    METHOD m_method;

    /*客户请求的目标文件的完整路径，其内容等于doc_root + m_url,doc_root是网络根目录*/
    char m_real_file[FILENAME_LEN];
    /*客户请求的目标文件的文件名*/
    char* m_url;
    /*HTTP协议版本号，我们仅支持HTTP/1.1*/
    char* m_version;
    /*主机名*/
    char* m_host;
    /*HTTP请求的消息体长度*/
    int m_content_length;
    /*HTTP请求是否要求保持连接*/
    bool m_linger;

    /*客户请求的目标文件被mmap到内存中的起始位置*/
    char* m_file_address;
    /*目标文件的状态。通过它我们可以判断文件是否存在，是否为目录，是否可读，并获取文件大小等信息*/
    struct stat m_file_stat;
    /*我们将采用writev来执行写操作，所以定义下面两个成员，其中m_iv_count表示被写内存块的数量*/
    struct iove m_iv[2];
    int m_iv_count;
};
#endif
