CXX ?= g++ -std=c++11

DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CXXFLAGS += -g
else
    CXXFLAGS += -O2

endif

server: main.cc  ./http/http_conn.cc  ./sql_poll/sql_poll.cc ./lock/lock.cc ./thread_poll/thread_poll.h
	g++ -std=c++11 -o server  $^ $(CXXFLAGS) -lpthread -lmysqlclient

clean:
	rm  -r server
