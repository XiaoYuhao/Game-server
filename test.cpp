#include<iostream>
#include<string>
using namespace std;
//#include"Game.h"
//#include"OnlineClient.h"
#include"Server.h"

int main(){
/*	Game g;
	g.Game_init();
	g.Show_map();
*/
/*	Database database;
	short userid;
	short score;
	string username="user";
	string password="123456";
	
	int res=database.login(username,password,userid);
	cout<<res<<endl;
	if(res>0){
		cout<<username<<" Login success!"<<"Userid:"<<userid<<endl;
		database.update_score(userid,88);
		database.get_score(userid,score);
		cout<<"Userid:"<<userid<<" new score:"<<score<<endl;
	}
	else{
		cout<<username<<" Login failed!"<<endl;
	}
	database.update_username(userid,"xiaoxiao");
*/
	daemon(1,1); 
	Server server;
	server.work();
	return 0;
}