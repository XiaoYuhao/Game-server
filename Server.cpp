#include"Server.h"
using namespace std;

Server::Server(int port){
	server_port=port;
	string log_file_name="Serverlog_";
	string current_time=get_current_time();
	log_file_name+=current_time;
	log_file_name+=string(".log");
	
	log_file_stream.open(log_file_name.c_str());
	if(!log_file_stream.is_open())
	{
		cerr<<"server log create error"<<endl;
		exit(EXIT_FAILURE);
	}
	print_log("server started");
	print_hint("server started");
}

void Server::print_err_log(string err_log){
	log_file_stream<<"ERROR: "<<get_current_time()<<": "<<err_log
				   <<" - "<<strerror(errno)<<endl;
	cout<<"Error: "<<get_current_time()<<": "<<err_log<<endl;
}

void Server::print_log(string log){
	log_file_stream<<get_current_time()<<": "<<log<<endl;
}

void Server::print_log(char const *log){
	log_file_stream<<get_current_time()<<": "<<log<<endl;
}
void Server::print_hint(string hint){
	cout<<hint<<endl;
}

void Server::startup(){
	socklen_t len;
	
	if((server_sock=socket(AF_INET,SOCK_DGRAM,0))==-1){
		print_err_log("Create server socket failed.");
		exit(1);
	}
	print_log("Create server socket success.");
	
	server.sin_family=AF_INET;
	server.sin_port=htons(server_port);
	server.sin_addr.s_addr=htonl(INADDR_ANY);
	
	if(bind(server_sock,(struct sockaddr *)&server,sizeof(server))==-1){
		print_err_log("Bind() error.");
		exit(1);
	}
	print_log("Bind() success.");
	
}

void Server::send_package(char *sendbuf,int len){
	socklen_t clen;
	clen=sizeof(client);
	int res=sendto(server_sock,sendbuf,len,0,(struct sockaddr *)&client,clen);
	if(res<0){
		print_err_log("sendto() failed.");
	}
}

int Server::newSession(){
	srand((int)time(0));
	int sess;
	while(1){
		sess=rand()%INT_MAX;
		if(sess_client.count(sess)){//如果session已经被占用
			continue;
		}
		else{
			break;
		}
	}
	return sess;
}
void Server::login(login_ask_package &p){
	string username(p.username);
	string password(p.password);
	short userid;
	int res=database.login(username,password,userid);
	
	if(res!=1){//登录错误
		login_reply_package rp(~p.func_package.package_seq,0,LOGIN_FAIL,0);//回复报文
		char sendbuf[BUFSIZE];
		memcpy(sendbuf,&rp,sizeof(rp));
		send_package(sendbuf,sizeof(rp));
		sprintf(temp_log,"Login Failed. Requset from IP:%s Port:%d",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
		print_log(temp_log);
	}
	else{	//登录成功
		if(id_session.count(userid)){			//此userid已存在session
			int old_sess=id_session[userid];	//获取原session值
			OnlineClient *old_oc;
			old_oc=sess_client[old_sess];
			if(inet_ntoa(old_oc->client.sin_addr)==inet_ntoa(client.sin_addr)&&old_oc->client.sin_port==client.sin_port){//同一个登录请求
				int sess=old_oc->session; 
				short score;
				database.get_score(userid,score);
				login_reply_package rp(sess_client[sess]->seq,sess,LOGIN_SUCCESS,(char)score);//回复报文
				char sendbuf[BUFSIZE];
				memcpy(sendbuf,&rp,sizeof(rp));
				send_package(sendbuf,sizeof(rp));
				sprintf(temp_log,"The Same login ask(Userid:%d session:%d). Requset from IP:%s Port:%d",userid,sess,inet_ntoa(client.sin_addr),ntohs(client.sin_port));
				print_log(temp_log);
			}
			else{
				int sess=newSession();				//获取新的session值
				id_session[userid]=sess;			//更新session值
				delete sess_client[old_sess];
				sess_client.erase(old_sess);		//删除原session值
				sess_client.insert(map<int,OnlineClient*>::value_type(sess,NULL));
				sess_client[sess]=new OnlineClient;
				sess_client[sess]->session=sess;
				sess_client[sess]->userid=userid;
				sess_client[sess]->seq=~p.func_package.package_seq;
				
				short score;
				database.get_score(userid,score);
				
				time_t old_time,new_time;
				old_time=sess_time[old_sess];
				new_time=time(0);
				sess_time.erase(old_sess);
				sess_time.insert(map<int,time_t>::value_type(sess,new_time));
				if(new_time-2>old_time){//旧session值最后报文时间old_time早于当前时间2秒，用户不在线（但是没有发退出请求）
					login_reply_package rp(sess_client[sess]->seq,sess,LOGIN_SUCCESS,(char)score);//回复报文
					char sendbuf[BUFSIZE];
					memcpy(sendbuf,&rp,sizeof(rp));
					send_package(sendbuf,sizeof(rp));
					sprintf(temp_log,"Login success(Userid:%d session:%d). Requset from IP:%s Port:%d",userid,sess,inet_ntoa(client.sin_addr),ntohs(client.sin_port));
					print_log(temp_log);
				}
				else{//旧session值用户仍然在线
					login_reply_package rp(sess_client[sess]->seq,sess,LOGIN_REPLACE,(char)score);//回复报文
					char sendbuf[BUFSIZE];
					memcpy(sendbuf,&rp,sizeof(rp));
					send_package(sendbuf,sizeof(rp));
					sprintf(temp_log,"Login success(Userid:%d session:%d) and replace the old user. Requset from IP:%s Port:%d",userid,sess,inet_ntoa(client.sin_addr),ntohs(client.sin_port));
					print_log(temp_log);
				}
				sess_client[sess]->client.sin_addr=client.sin_addr;
				sess_client[sess]->client.sin_port=client.sin_port;
			}
		}
		else{									//此userid不存在session
			int sess=newSession();
			id_session.insert(map<short,int>::value_type(userid,sess));
			//id_session.insert(userid);
			id_session[userid]=sess;
			//sess_client.insert(sess);
			sess_client.insert(map<int,OnlineClient*>::value_type(sess,NULL));
			sess_client[sess]=new OnlineClient;
			sess_client[sess]->session=sess;
			sess_client[sess]->userid=userid;
			sess_client[sess]->seq=~p.func_package.package_seq;
			short score;
			database.get_score(userid,score);
			login_reply_package rp(sess_client[sess]->seq,sess,LOGIN_SUCCESS,(char)score);//回复报文
			char sendbuf[BUFSIZE];
			memcpy(sendbuf,&rp,sizeof(rp));
			send_package(sendbuf,sizeof(rp));
			sprintf(temp_log,"Login success(Userid:%d session:%d). Requset from IP:%s Port:%d",userid,sess,inet_ntoa(client.sin_addr),ntohs(client.sin_port));
			print_log(temp_log);
			
			sess_client[sess]->client.sin_addr=client.sin_addr;
			sess_client[sess]->client.sin_port=client.sin_port;
		}
	}
}

void Server::logout(logout_ask_package &p){
	int sess=ntohl(p.session);
	short userid;
	if(sess_client.count(sess)){//存在对应的session值
		OnlineClient *oc;
		oc=sess_client[sess];
		userid=oc->userid;
		delete oc;				//释放掉对应的Onlineclient结构
		sess_client.erase(sess);//释放掉session值
		sess_time.erase(sess);	//删除对应的定时器
		sprintf(temp_log,"userid:%d,seesion:%d logout success. Requset from IP:%s Port:%d",userid,sess,inet_ntoa(client.sin_addr),ntohs(client.sin_port));
		print_log(temp_log);
	}
	logout_reply_package rp(0,LOGOUT_SUCCESS);
	char sendbuf[BUFSIZE];
	memcpy(sendbuf,&rp,sizeof(rp));
	send_package(sendbuf,sizeof(rp));
	
}
void Server::new_game(game_ask_package &p){
	int sess=ntohl(p.session);
	if(sess_client.count(sess)){//存在对应的session值
		OnlineClient *oc;
		oc=sess_client[sess];
		
		if(oc->seq==p.func_package.package_seq){//收到的是新的报文
			oc->g.Game_init();
			sprintf(temp_log,"Userid:%d start a new game:",oc->userid);
			print_log(temp_log);
			oc->g.Print_map(log_file_stream);
			oc->seq=~p.func_package.package_seq;
		}
		else{//收到的是重复报文
			//不执行相关操作
			print_log("The same package asking to init a new game.");
		}
		//但是报文还是要发的...
		game_reply_package rp(oc->seq,GAME_SUCCESS);
		char sendbuf[BUFSIZE];
		memcpy(sendbuf,&rp,sizeof(rp));
		send_package(sendbuf,sizeof(rp));
	}
	else{//非法session值
		sprintf(temp_log,"Invalid session:%d ask to start new game.",sess);
		print_log(temp_log);
		game_reply_package rp(p.func_package.package_seq,INVALID);
		char sendbuf[BUFSIZE];
		memcpy(sendbuf,&rp,sizeof(rp));
		send_package(sendbuf,sizeof(rp));
	}
}

void Server::game_single(single_ask_package &p){
	int sess=ntohl(p.session);
	int x=ntohs(p.x);
	int y=ntohs(p.y);
	if(sess_client.count(sess)){
		OnlineClient *oc;
		oc=sess_client[sess];
		
		if(oc->seq==p.func_package.package_seq){//收到的是新的报文
			int res=oc->g.Single_xy(x,y);
			if(res==0){
				oc->g.setRes_Pla(NO_PLANE,NO_PLANE);
			}
			else if(res==1){
				oc->g.setRes_Pla(PLANE_BODY,PLANE1);
			}
			else if(res==2){
				oc->g.setRes_Pla(PLANE_BODY,PLANE2);
			}
			else if(res==3){
				oc->g.setRes_Pla(PLANE_BODY,PLANE3);
			}
			else{
				oc->g.setRes_Pla(PLANE_HEAD,res-PLANE_HEAD);
			}
			oc->seq=~p.func_package.package_seq;
			sprintf(temp_log,"Userid:%d Session:%d play single_xy(x:%d,y:%d).Result:%d.Plane:%d.Score:%d.",oc->userid,oc->session,x,y,oc->g.getRes(),oc->g.getPla(),oc->g.getScore());
			print_log(temp_log);
		}
		else{//收到的是重复报文
			//不执行相关操作
			print_log("The same package asking to play single_xy");
		}
		single_reply_package rp(oc->seq,oc->g.getRes(),oc->g.getPla(),oc->g.getScore());
		char sendbuf[BUFSIZE];
		memcpy(sendbuf,&rp,sizeof(rp));
		send_package(sendbuf,sizeof(rp));
		
	}
	else{
		sprintf(temp_log,"Invalid session:%d ask to play game.",sess);
		print_log(temp_log);
		single_reply_package rp(p.func_package.package_seq,INVALID,0,0);
		char sendbuf[BUFSIZE];
		memcpy(sendbuf,&rp,sizeof(rp));
		send_package(sendbuf,sizeof(rp));
	}
}

void Server::game_double(double_ask_package &p){
	int sess=ntohl(p.session);
	int x1=ntohs(p.x1);
	int y1=ntohs(p.y1);
	int x2=ntohs(p.x2);
	int y2=ntohs(p.y2);
	if(sess_client.count(sess)){
		OnlineClient *oc;
		oc=sess_client[sess];
		int userid=oc->userid;
		if(oc->seq==p.func_package.package_seq){//收到的是新的报文
			int res=oc->g.Double_xy(x1,y1,x2,y2);
			if(res==0){//未猜中飞机
				oc->g.setRes_Pla(WRONG,NO_PLANE);
			}
			else if(res==1){//猜中一号飞机
				oc->g.setRes_Pla(RIGHT,PLANE1);
			}
			else if(res==2){//猜中二号飞机
				oc->g.setRes_Pla(RIGHT,PLANE2);
			}
			else if(res==3){//猜中三号飞机
				oc->g.setRes_Pla(RIGHT,PLANE3);
			}
			else if(res>3){//猜中第三架飞机，游戏结束
				oc->g.setRes_Pla(GAME_OVER,res-OVER);
				short old_score,new_score;
				database.get_score(userid,old_score);
				new_score=oc->g.getScore();
				if(new_score>old_score){	//如果新分数比原分数高则更新
					database.update_score(userid,new_score);
				}
				
			}
			oc->seq=~p.func_package.package_seq;
			sprintf(temp_log,"Userid:%d Session:%d play double_xy(x1:%d,y1:%d)(x2:%d,y2:%d).Result:%d.Plane:%d.Score:%d.",oc->userid,oc->session,x1,y1,x2,y2,oc->g.getRes(),oc->g.getPla(),oc->g.getScore());
			print_log(temp_log);
		}
		else{//收到的是重复报文
			//不执行相关操作
			print_log("The same package asking to play double_xy");
		}
		
		double_reply_package rp(oc->seq,oc->g.getRes(),oc->g.getPla(),oc->g.getScore());
		char sendbuf[BUFSIZE];
		memcpy(sendbuf,&rp,sizeof(rp));
		send_package(sendbuf,sizeof(rp));
	}
	else{
		sprintf(temp_log,"Invalid session:%d ask to play game.",sess);
		print_log(temp_log);
		double_reply_package rp(p.func_package.package_seq,INVALID,0,0);
		char sendbuf[BUFSIZE];
		memcpy(sendbuf,&rp,sizeof(rp));
		send_package(sendbuf,sizeof(rp));
	}
	
}

void Server::reply_alive(alive_ask_package &p){
	int sess=ntohl(p.session);
	sprintf(temp_log,"Alive ask form session:%d.",sess);
	print_log(temp_log);
	if(sess_client.count(sess)){//session为合法的session值
		OnlineClient *oc;
		oc=sess_client[sess];
		if(oc->seq==p.func_package.package_seq){//收到的是新的报文
			oc->seq=~p.func_package.package_seq;
		}
		else{//收到的是重复报文
			//不执行相关操作
			print_log("The same alive_aks_package...");
		}
		print_log("This session is valid.");
		alive_reply_package rp(oc->seq,VALID);
		char sendbuf[BUFSIZE];
		memcpy(sendbuf,&rp,sizeof(rp));
		send_package(sendbuf,sizeof(rp));
		
	}
	else{//session为非法的session值
		print_log("This session is invalid.");
		alive_reply_package rp(p.func_package.package_seq,INVALID);
		char sendbuf[BUFSIZE];
		memcpy(sendbuf,&rp,sizeof(rp));
		send_package(sendbuf,sizeof(rp));
	}
}
void Server::reply(char *recvbuf,int len){
	char type=recvbuf[0];
	if(type==LOGIN_REQ){
		print_log("This is a Login_req package...");
		login_ask_package p;
		memcpy(&p,recvbuf,len);
		login(p);
	}
	else if(type==NAME_REQ){
		//暂时不支持此功能
	}
	else if(type==PW_REQ){
		//暂时不支持此功能
	}
	else if(type==GAME_REQ){
		print_log("This is a Game_req package...");
		game_ask_package p1;
		int sess=p1.session;
		sess_time[sess]=time(0);
		memcpy(&p1,recvbuf,len);
		new_game(p1);
	}
	else if(type==SINGLE_REQ){
		print_log("This is a Single_play_req package...");
		single_ask_package p2;
		int sess=p2.session;
		sess_time[sess]=time(0);
		memcpy(&p2,recvbuf,len);
		game_single(p2);
	}
	else if(type==DOUBLE_REQ){
		print_log("This is a Double_play_req package...");
		double_ask_package p3;
		int sess=p3.session;
		sess_time[sess]=time(0);
		memcpy(&p3,recvbuf,len);
		game_double(p3);
	}
	else if(type==ALIVE_REQ){
		print_log("This is a Alive_ask package...");
		alive_ask_package p4;
		int sess=p4.session;
		sess_time[sess]=time(0);
		memcpy(&p4,recvbuf,len);
		reply_alive(p4);
	}
	else if(type==LOGOUT_REQ){
		print_log("This is a Logout_req package...");
		logout_ask_package p5;
		int sess=p5.session;
		sess_time[sess]=time(0);
		memcpy(&p5,recvbuf,len);
		logout(p5);
	}
/*	switch (type){
		case LOGIN_REQ:
			sprintf(temp_log,"This is a Login_req package...");
			print_log(temp_log);
			login_ask_package p;
			memcpy(&p,recvbuf,len);
			login(p);
			break;
		case NAME_REQ:
			break;
		case PW_REQ:
			break;
		case GAME_REQ:
			sprintf(temp_log,"This is a Game_req package...");
			print_log(temp_log);
			game_ask_package p1;
			memcpy(&p1,recvbuf,len);
			new_game(p1);
			break;
		case SINGLE_REQ:
			sprintf(temp_log,"This is a Single_play_req package...");
			print_log(temp_log);
			single_ask_package p2;
			memcpy(&p2,recvbuf,len);
			game_single(p2);
			break;
		case DOUBLE_REQ:
			sprintf(temp_log,"This is a Double_play_req package...");
			print_log(temp_log);
			double_ask_package p3;
			memcpy(&p3,recvbuf,len);
			game_double(p3);
			break;
		
		case ALIVE_REQ:
			break;
		default:
			break;
	}*/
}

void Server::work(){
	startup(); //服务器启动
	socklen_t len;
	len=sizeof(client);
	int num;
	char recvbuf[BUFSIZE];
	while(1){
		num=recvfrom(server_sock,recvbuf,BUFSIZE,0,(struct sockaddr *)&client,&len);
		if(num<=0){
			print_err_log("recvfrom() error.");
			exit(1);
		}
		sprintf(temp_log,"Server got a message from a client whose ip is %s,port is %d.",inet_ntoa(client.sin_addr),htons(client.sin_port));
		string content(temp_log);
		print_log(content);
		reply(recvbuf,num);
		
	}
}