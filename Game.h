#ifndef _GAME_HEADER_

#define _GAME_HEADER_

const int MAP_SIZE=10;

struct Plane{
	int head_x,head_y;
	int tail_x,tail_y;
	int mode;
}

class Game
{
	int map[MAP_SIZE][MAP_SIZE]; 	//游戏的地图，存放飞机信息
	Plane plane[3];
	int score;			//游戏的分数
	void Game_init(); 	//初始化游戏的地图
	void Show_map();
};


#endif
