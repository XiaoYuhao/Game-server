#include"Database.h"

string encrypt(string password)
{
    MD5 md5(password);
    return md5.md5();
}

Database::Database(){
	string log_file_name="Databaselog_";
	string current_time=get_current_time();
	log_file_name+=current_time;
	log_file_name+=string(".log");
	log_stream.open(log_file_name.c_str());
	if(!log_stream.is_open()){
		cerr<<"DB log create error"<<endl;
		exit(EXIT_FAILURE);
	}
	
	if((mysql=mysql_init(NULL))==NULL){
		print_log("mysql_init failed");
	}
	
	if(mysql_real_connect(mysql,"localhost",db_username.c_str(),db_password.c_str(),db_name.c_str(),0,NULL,0)==NULL){
		cout<<"mysql_real_connect failed("<<mysql_error(mysql)<<")"<<endl;
		print_log("mysql_real_connect failed");
	}
	
	mysql_set_character_set(mysql,"gbk");
}

Login_status Database::login(string username,string password,short &userid){
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	sprintf(query,
			"select count(*) "\
			"from %s "\
			"where username=\'%s\'",
			user_table_name.c_str(),
			username.c_str());

	if(send_query(query,&result)==ERROR){
		print_log("login select query failed");
		mysql_free_result(result);
		return login_error;
	}
	
	row=mysql_fetch_row(result);
	if(strcmp(row[0],"1")){
		print_log("username not found");
		mysql_free_result(result);
		return fail_username;
	}
	
	string encrypted_password=encrypt(password);
	sprintf(query,
			"select count(*) "\
			"from %s "\
			"where username=\'%s\' and password=\'%s\'",
			user_table_name.c_str(),
			username.c_str(),
			encrypted_password.c_str());
			
	if(send_query(query,&result)==ERROR)
	{
		print_log("login select query failed");
		mysql_free_result(result);
		return login_error;
	}
	
	row=mysql_fetch_row(result);
	if(strcmp(row[0],"1")){
		print_log("password error");
		mysql_free_result(result);
		return fail_password;
	}
	
	print_log("username matches password");
	
	userid=get_userid(username);
	if(userid<0){
		print_log("login get userid query failed");
		mysql_free_result(result);
		return login_error;
	}
	time_t current_time=time(0);
	print_log("update user login time to current time");
	sprintf(query,
			"update %s "\
			"set logintime=FROM_UNIXTIME(%d) "\
			"where id= %d;",
			user_table_name.c_str(),
			current_time,
			userid);
	
	if(send_update(query)==ERROR){
		print_log("login update query failed");
		mysql_free_result(result);
		return login_error;
	}
	mysql_free_result(result);
	
	return login_success;
}

int Database::update_password(short userid,string password){
	string encrypted_password=encrypt(password);
	sprintf(query,
			"update %s "\
			"set password = \'%s\' "\
			"where id = %d;",
			user_table_name.c_str(),
			encrypted_password.c_str(),
			userid);

	if(send_update(query)==ERROR){
		print_log("update password query failed");
		return ERROR;
	}
	print_log("update username query success");
	return OK;
}

int Database::update_username(short userid,string username){
	sprintf(query,
			"update %s "\
			"set username = \'%s\' "\
			"where id = %d;",
			user_table_name.c_str(),
			username.c_str(),
			userid);

	if(send_update(query)==ERROR){
		print_log("update username query failed");
		return ERROR;
	}
	print_log("updata username query success");
	return OK;
}

int Database::get_score(short userid,short &score){
	sprintf(query,
			"select score "\
			"from %s "\
			"where id = %d;",
			user_table_name.c_str(),
			userid);
	
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	if(send_query(query,&result)==ERROR){
		print_log("score select query failed");
		mysql_free_result(result);
		return ERROR;
	}
	
	row=mysql_fetch_row(result);
	string res=row[0];
	score=atoi(res.c_str());
	mysql_free_result(result);
	return OK;
}

int Database::update_score(short userid,int new_score){
	sprintf(query,
			"update %s "\
			"set score= %d "\
			"where id = %d;",
			user_table_name.c_str(),
			new_score,
			userid);		
	
	if(send_update(query)==ERROR){
		print_log("update score query failed");
		return ERROR;
	}
	print_log("update score query success");
	return OK;
}


void Database::print_log(string content){
	log_stream<<"Time: "<<get_current_time()<<": "<<content<<endl;
}

int Database::send_query(const char* query,MYSQL_RES **result){
	print_log(query);
	if(mysql_query(mysql,query))
	{
		cout<<"mysql_query failed("<<mysql_error(mysql)<<")"<<endl;
		print_log("mysql_query failed");
		return ERROR;
	}
	
	if((*result=mysql_store_result(mysql))==NULL){
		print_log("mysql_store_result failed");
		return ERROR;
	}
	return OK;
}

int Database::send_update(const char* query){
	print_log(query);
	if(mysql_query(mysql,query))
	{
		cout<<"mysql_query failed("<<mysql_error(mysql)<<")"<<endl;
		print_log("mysql_query failed");
		return ERROR;
	}
	return OK;
}

short Database::get_userid(string username){
	sprintf(query,
			"select id "\
			"from %s "\
			"where username = \'%s\';",
			user_table_name.c_str(),
			username.c_str());
	
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	if(send_query(query,&result)==ERROR){
		print_log("get_userid query failed");
		return ERROR;
	}
	
	row=mysql_fetch_row(result);
	string res=row[0];
	short userid=atoi(res.c_str());
	
	mysql_free_result(result);
	return userid;
}


