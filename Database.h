#ifndef _DATABASE_HEADER_

#define _DATABASE_HEADER_

#include<iostream>
#include<iomanip> 
#include<string>
#include<mysql.h> 
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<fstream>
#include<iostream>
#include<exception>
#include<map>
#include<set>
#include<utility>
#include<functional>
#include<time.h>
#include"tools.h"
#include"md5.h"

#define ERROR -1
#define OK 1

using namespace std;

const string db_username="u1652195";
const string db_password="u1652195";
const string db_name="db1652195";

const string user_table_name="USER";

typedef int Status;
enum Login_status{login_success=1,fail_username=-1,fail_password=-2,login_error=0};

class Database{
private:
	MYSQL *mysql;
	char query[2048];
	char temp_log[128];
	ofstream log_stream;

public:
	Database();
	~Database(){
		mysql_close(mysql);
	}
	
	Login_status login(string username,string password,short &userid);
	int update_password(short userid,string password);
	int update_username(short userid,string username);
	int get_score(short userid,short &score);
	int update_score(short userid,int new_score);
	//bool reg(string username,string password);
	
private:
	int send_query(const char* query,MYSQL_RES **result);
	int send_update(const char* query);
	short get_userid(string username);
	void print_log(string content);
	
};


#endif