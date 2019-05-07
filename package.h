#ifndef _PACKAGE_HEADER_

#define _PACKAGE_HEADER_

#include<cstdlib>
#include<string.h>
#include<string>
#include<arpa/inet.h>

const char LOGIN_REQ			=0x11;		//登录请求报文	
const char LOGIN_REPLY			=0x71;		//登录回复报文
const char LOGIN_SUCCESS 		=0x00;		//登录成功
const char LOGIN_FAIL			=0x01;		//登录失败，用户名或密码错误
const char LOGIN_REPLACE		=0x02;		//登录成功并且挤掉重复用户

const char NAME_REQ				=0x12;		//改名请求报文
const char NAME_REPLY			=0x72;		//改名回复报文
const char NAME_SUCCESS			=0x00;		//改名成功
const char NAME_FAIL			=0x01;		//改名失败

const char PW_REQ				=0x13;		//修改密码请求报文
const char PW_REPLY				=0x73;		//修改密码回复报文
const char PW_SUCCESS			=0x00;		//修改密码成功
const char PW_FAIL				=0x01;		//修改密码失败

const char LOGOUT_REQ			=0x14;		//退出登录请求报文
const char LOGOUT_REPLY			=0x74;		//退出登录回复报文
const char LOGOUT_SUCCESS		=0x01;		//退出登录成功

const char GAME_REQ		    	=0x20;		//开始游戏请求报文
const char GAME_REPLY			=0x80;		//开始游戏回复报文
const char GAME_SUCCESS			=0x00;		//初始化游戏成功
const char GAME_FAIL			=0x01;		//初始化游戏失败

const char SINGLE_REQ			=0x21;		//单坐标操作请求报文
const char SINGLE_REPLY			=0x81;		//单坐标操作回复报文
const char NO_PLANE				=0x00;		//没有猜中飞机
const char PLANE_BODY			=0x01;		//猜中飞机机身
const char PLANE_HEAD			=0x02;		//猜中飞机头部
const char PLANE1				=0x01;		//猜中1号飞机
const char PLANE2				=0x02;		//猜中2号飞机
const char PLANE3				=0x03;		//猜中3号飞机

const char DOUBLE_REQ			=0x22;		//双坐标操作请求报文
const char DOUBLE_REPLY			=0x82;		//双坐标操作回复报文
const char RIGHT				=0x00;		//是
const char WRONG				=0x01;		//否
const char GAME_OVER			=0x88;		//游戏结束（完全猜中第三架飞机）

const char ALIVE_REQ			=0x30;		//心跳报文
const char ALIVE_REPLY			=0x90;		//心跳回复报文

const char EMPTY				=0x00;		//填充
const char VALID				=0x08;		//合法请求（未掉线）
const char INVALID				=0xff;		//无效请求（掉线）

struct functional_package{
	char package_type;
	char package_seq;
	short package_length;
};

/*
			登录请求报文
| 0x11  | seq	|	0x003C		|
|			username	...		|
|			username			|
|			password	...		|
|			password			|
*/
struct login_ask_package{
	functional_package func_package;
	char username[28];
	char password[28];
	login_ask_package(){}
	login_ask_package(string name,string pw,char seq){
		func_package.package_type=LOGIN_REQ;
		func_package.package_seq=seq;
		func_package.package_length=htons(0x003C);
		strcpy(username,name.c_str());
		strcpy(password,pw.c_str());
	}
};
/*
			登录回复报文
| 0x71  | ack	|	0x0008		|
|       session(int)            |
| result| score	|   empty       |
*/
struct login_reply_package{
	functional_package func_package;
	int session;
	char result;
	char score;
	char empty_area[2];
	login_reply_package(){}
	login_reply_package(char ack,int sess,char res,char sco){
		func_package.package_type=LOGIN_REPLY;
		func_package.package_seq=ack;
		func_package.package_length=htons(0x0008);
		session=htonl(sess);
		result=res;
		score=sco;
	}
};
/*
			改名请求报文
| 0x12  | seq	|	0x0020		|
|       session(int)            |
| 	        username   ...      |
|			username			|
*/
struct update_ask_username{
	functional_package func_package;
	int session;
	char username[28];
	update_ask_username(){}
	update_ask_username(char seq,int sess,string name){
		func_package.package_type=NAME_REQ;
		func_package.package_seq=seq;
		func_package.package_length=htons(0x0020);
		session=htonl(sess);
		strcpy(username,name.c_str());
	}
};
/*
			改名回复报文
| 0x72  | ack	|	0x0008		|
| result|       empty           |
*/
struct update_reply_username{
	functional_package func_package;
	char result;
	char empty_area[3];
	update_reply_username(){}
	update_reply_username(char ack,char res){
		func_package.package_type=NAME_REPLY;
		func_package.package_seq=ack;
		func_package.package_length=htons(0x0008);
		result=res;
	}
};
/*
			修改密码请求报文
| 0x13  | seq	|	0x0020		|
|       session(int)            |
| 	        password   ...      |
|			password			|
*/
struct update_ask_password{
	functional_package func_package;
	int session;
	char password[28];
	update_ask_password(){}
	update_ask_password(char seq,int sess,string pw){
		func_package.package_type=PW_REQ;
		func_package.package_seq=seq;
		func_package.package_length=htons(0x0020);
		session=htonl(sess);
		strcpy(password,pw.c_str());
	}
};
/*
			修改密码回复报文
| 0x73  | ack	|	0x0008		|
| result|        empty          |
*/
struct update_reply_password{
	functional_package func_package;
	char result;
	char empty_area[3];
	update_reply_password(){}
	update_reply_password(char ack,char res){
		func_package.package_type=PW_REPLY;
		func_package.package_seq=ack;
		func_package.package_length=htons(0x0008);
		result=res;
	}
};

/*
        退出登录请求报文
| 0x14  | seq	|	0x0008		|
|       session(int)            |
*/
struct logout_ask_package{
    functional_package func_package;
    int session;
    logout_ask_package(){}
    logout_ask_package(char seq,int sess){
        func_package.package_type=LOGOUT_REQ;
        func_package.package_seq=seq;
        func_package.package_length=htons(0x0008);
        session=htonl(sess);
    }
};

/*
        退出登录回复报文
| 0x74  | ack	|	0x0008		|
| result|                       |
*/
struct logout_reply_package{
    functional_package func_package;
    char result;
    logout_reply_package(){}
    logout_reply_package(char ack,char res){
        func_package.package_type=LOGOUT_REPLY;
        func_package.package_seq=ack;
        func_package.package_length=htons(0x0008);
        result=res;
    }
};

/*
			请求新游戏
| 0x20  | seq	|	0x0008		|
|       session(int)            |
*/
struct game_ask_package{
	functional_package func_package;
	int session;
	game_ask_package(){}
	game_ask_package(char seq,int sess){
		func_package.package_type=GAME_REQ;
		func_package.package_seq=seq;
		func_package.package_length=htons(0x0008);
		session=htonl(sess);
	}
};
/*
			回复新游戏
| 0x80  | ack	|	0x0008		|
| result|   empty               |
*/
struct game_reply_package{
	functional_package func_package;
	char result;
	char empty_area[3];
	game_reply_package(){}
	game_reply_package(char ack,char res){
		func_package.package_type=GAME_REPLY;
		func_package.package_seq=ack;
		func_package.package_length=htons(0x0008);
		result=res;
	}
};

/*
			单坐标操作请求
| 0x21  | seq	|	0x000c		|
|       session(int)            |
|       x       |       y       |
*/
struct single_ask_package{
	functional_package func_package;
	int session;
	short x;
	short y;
	single_ask_package(){}
	single_ask_package(char seq,int sess,short xx,short yy){
		func_package.package_type=SINGLE_REQ;
		func_package.package_seq=seq;
		func_package.package_length=htons(0x000c);
		session=htonl(sess);
		x=htons(xx);
		y=htons(yy);
	}
};

/*
			单坐标操作回复
| 0x81  | ack	|	0x0008		|
| result| plane | score |       |
*/
struct single_reply_package{
	functional_package func_package;
	char result;
	char plane;	//猜中哪一架飞机
	char score;
	char empty_area;
	single_reply_package(){}
	single_reply_package(char ack,char res,char pla,char sco){
		func_package.package_type=SINGLE_REPLY;
		func_package.package_seq=ack;
		func_package.package_length=htons(0x0008);
		result=res;
		plane=pla;
		score=sco;
	}
};

/*
			双坐标操作请求
| 0x22  | seq	|	0x000c		|
|       session(int)            |
|       x1      |       y1      |
|       x2      |       y2      |
*/
struct double_ask_package{
	functional_package func_package;
	int session;
	short x1;
	short y1;
	short x2;
	short y2;
	double_ask_package(){}
	double_ask_package(char seq,int sess,short xx1,short yy1,short xx2,short yy2){
		func_package.package_type=DOUBLE_REQ;
		func_package.package_seq=seq;
		func_package.package_length=htons(0x0010);
		session=htonl(sess);
		x1=htons(xx1);
		y1=htons(yy1);
		x2=htons(xx2);
		y2=htons(yy2);
	}
};

/*
			双坐标操作回复
| 0x82  | ack	|	0x0008		|
| result| plane | score |       |
*/
struct double_reply_package{
	functional_package func_package;
	char result;
	char plane;	//猜中哪一架飞机
	char score;
	char empty_area;
	double_reply_package(){}
	double_reply_package(char ack,char res,char pla,char sco){
		func_package.package_type=DOUBLE_REPLY;
		func_package.package_seq=ack;
		func_package.package_length=htons(0x0008);
		result=res;
		plane=pla;
		score=sco;
	}
};

/*
			心跳包
| 0x30  | ack	|	0x0008		|
| 			session 			|
*/
struct alive_ask_package{
	functional_package func_package;
	int session;
	alive_ask_package(){}
	alive_ask_package(char ack,int sess){
		func_package.package_type=ALIVE_REQ;
		func_package.package_seq=ack;
		func_package.package_length=htons(0x0008);
		session=htonl(sess);
	}
};
/*
			心跳回复包
| 0x90  | ack	|	0x0008		|
| result|		   			    | 
*/
struct alive_reply_package{
	functional_package func_package;
	char result;
	char empty_area[3];
	alive_reply_package(){}
	alive_reply_package(char ack,char res){
		func_package.package_type=ALIVE_REPLY;
		func_package.package_seq=ack;
		func_package.package_length=htons(0x0008);
		result=res;
	}
};


#endif