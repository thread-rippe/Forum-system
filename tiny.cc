#include "DK.cc"
#include "./sql/sql_method.cc"
void read_requesthdrs(rio_t *rp);
bool parse_uri(string& uri, string& filename, string& cgiargs);
void serve_static(int fd, string& filename, int filesize);
void get_filetype(string& filename, string& filetype);
void serve_dynamic(int fd, string& filename, string& cgiargs);
void clienterror(int fd, const string& cause, const string& errnum, const string& shortmsg, const string& longmsg);
void doit(int fd);

const int MAX_THREAD = 8;

const string host = "192.168.25.252";
const string user = "root";
const string passwd = "123456";
const string database = "try";

sbuf<int> usr_buf(MAX_THREAD);
int main(int argc, char **argv){
	int listenfd, connfd;
	char hostname[MAXLINE], port[MAXLINE];
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;

	if(argc != 2){
		cerr << "usage: " << argv[0] << " <port>" << endl;
		exit(1);
	}

	for(int i = 0; i < MAX_THREAD; ++i){
		thread temp(doit, 1);
		temp.detach();
        cout << "线程创建" << i << endl;
	}

	listenfd = Open_listenfd(argv[1]);
	while(1){
		clientlen = sizeof(clientaddr);
		connfd = Accept(listenfd, reinterpret_cast<SA *>(&clientaddr), &clientlen);
		Getnameinfo(reinterpret_cast<SA *>(&clientaddr), clientlen, hostname, MAXLINE, port, MAXLINE, 0);
		cout << "连接到：" << hostname << " " << port << " " << endl;
		//cout << "新的连接！！" << endl << endl;
		usr_buf.insert(connfd);
	}
	return 0;
}

void doit(int _){
	while(1){
		int temp = usr_buf.get();
        MYSQL mysql;
        if(mysql_init(&mysql) == nullptr){
            cout << "初始化错误" << endl;
            exit(1);
        }
        if(!mysql_real_connect(&mysql, host.c_str(), user.c_str(), passwd.c_str(), database.c_str(), 0, nullptr, 0)){
            cout << "连接失败" << endl;
            exit(1);
        }
		Mission cur_m(temp, &mysql);
		cur_m.start();
        mysql_close(&mysql);
	}
}
