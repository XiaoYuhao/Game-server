#ifndef _ONLINECLIENT_HEADER_

#define _ONLINECLIENT_HEADER_

#include"Game.h"

class OnlineClient{
public:
	Game g;
	int session;
	int userid;
	char seq;
	
	struct sockaddr_in client;
	OnlineClient(){};
	~OnlineClient(){};
};

#endif