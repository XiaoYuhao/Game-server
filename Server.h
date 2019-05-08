#ifndef _SERVER_HEADER_

#define _SERVER_HEADER_

#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <exception>
#include <map>
#include <set>
#include <utility>
#include <functional>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>
#include <netdb.h>
#include <limits.h>
#include <assert.h>
#include"tools.h"
#include"OnlineClient.h"
#include"package.h"
#include"Database.h"
using namespace std;

const int BUFSIZE=512;

class Server{
private:
	int server_port;
	int server_sock;
	struct sockaddr_in server;				//服务器信息
	struct sockaddr_in client;				//当前处理的数据报来源client
	char temp_log[256];
	ofstream log_file_stream;
	
	Database database; 						//数据库实例化对象
	
	map<short,int> id_session;
	map<int,OnlineClient*> sess_client;
	map<int,time_t> sess_time;
	
private:
	void print_err_log(string err_log);
	void print_log(string log);
	void print_log(char const *log);
	void print_hint(string hint);
	
	void startup();
	void reply(char *recvbuf,int len);
	void send_package(char *sendbuf,int len);
	void login(login_ask_package &p);
	void logout(logout_ask_package &p);
	void new_game(game_ask_package &p);
	void game_single(single_ask_package &p);
	void game_double(double_ask_package &p);
	void reply_alive(alive_ask_package &p);
	int newSession();
	
public:
	Server(int server_portNum=23233);
	~Server(){
		for(auto i=sess_client.begin();i!=sess_client.end();++i){
			delete i->second;
			i->second=NULL;
		}
		print_log("Server closed.");
		log_file_stream.close();
	}
	void  work();
	
};



#endif