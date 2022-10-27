server: tiny.o 
	g++ -std=c++11 -o server tiny.o -lpthread -L/usr/lib64/mysql -lmysqlclient

tiny.o: tiny.cc DK.cc ./sql/sql_method.cc
	g++ -std=c++11 -c tiny.cc
