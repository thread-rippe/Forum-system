server: tiny.o add_html
	g++ -std=c++11 -o server tiny.o -lpthread -L/usr/lib64/mysql -lmysqlclient

tiny.o: tiny.cc DK.cc ./sql/sql_method.cc
	g++ -std=c++11 -c tiny.cc

add_html: add_html.o
	g++ -std=c++11 -o add_html add_html.o

add_html.o: add_html.cc
	g++ -std=c++11 -c add_html.cc
