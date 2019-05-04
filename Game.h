#ifndef _GAME_HEADER_

#define _GAME_HEADER_
#include<fstream>
using namespace std;
const int MAP_SIZE=10;
const int PLANE_NUM=3;

const int PLANE_NO=0;
const int PLANEBODY=0;
const int PLANEHEAD=3;
const int OVER=4;

struct Plane{
	int head_x,head_y;
	int tail_x,tail_y;
	int mode;
};

class Game
{
	int map[MAP_SIZE][MAP_SIZE]; 	//游戏的地图，存放飞机信息
	Plane plane[PLANE_NUM];
	int score;						//游戏的分数
	char result,planeno;			//操作的结果以及对应的飞机
	int plane_flag[PLANE_NUM];

public:
	void Game_init(); 				//初始化游戏的地图
	void Show_map() const;			//打印游戏地图
	void Print_map(ofstream &log_stream) const;
	int Single_xy(int x,int y);		//猜单坐标
	int Double_xy(int x1,int y1,int x2,int y2);		//猜双坐标
	char getScore();
	void setRes_Pla(char res,char pla);
	char getRes();
	char getPla();
};


#endif
