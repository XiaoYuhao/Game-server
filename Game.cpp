#include<iostream>
#include<stdlib.h>
#include<time.h>
#include"Game.h"

int _testone(int **map,int x,int y){
	if(x<0||x>9||y<0||y>9){
		return 0;
	}
	if(map[i][j]!=0){
		return 0;
	}
	return 1;
}

int _test1(int **map,int x,int y,int color){
	if(_testone(map,x,y)==0)return 0;
	if(_testone(map,x+1,y-2)==0)return 0;
	if(_testone(map,x+1,y-1)==0)return 0;
	if(_testone(map,x+1,y)==0)return 0;
	if(_testone(map,x+1,y+1)==0)return 0;
	if(_testone(map,x+1,y+2)==0)return 0;
	if(_testone(map,x+2,y)==0)return 0;
	if(_testone(map,x+3,y-1)==0)return 0;
	if(_testone(map,x+3,y)==0)return 0;
	if(_testone(map,x+3,y+1)==0)return 0;
	
	map[x][y]=color;
	map[x+1][y-2]=color;
	map[x+1][y-1]color;
	map[x+1][y]=color;
	map[x+1][y+1]=color;
	map[x+1][y+2]=color;
	map[x+2][y]=color;
	map[x+3][y-1]=color;
	map[x+3][y]=color;
	map[x+3][y+1]=color;
	return 1;
}
int _test2(int **map,int x,int y,int color){
	if(_testone(map,x,y)==0)return 0;
	if(_testone(map,x-1,y-2)==0)return 0;
	if(_testone(map,x-1,y-1)==0)return 0;
	if(_testone(map,x-1,y)==0)return 0;
	if(_testone(map,x-1,y+1)==0)return 0;
	if(_testone(map,x-1,y+2)==0)return 0;
	if(_testone(map,x-2,y)==0)return 0;
	if(_testone(map,x-3,y-1)==0)return 0;
	if(_testone(map,x-3,y)==0)return 0;
	if(_testone(map,x-3,y+1)==0)return 0;
	
	map[x][y]=color;
	map[x-1][y-2]=color;
	map[x-1][y-1]=color;
	map[x-1][y]=color;
	map[x-1][y+1]=color;
	map[x-1][y+2]=color;
	map[x-2][y]=color;
	map[x-3][y-1]=color;
	map[x-3][y]=color;
	map[x-3][y+1]=color;
	return 1;
}
int _test3(int **map,int x,int y,int color){
	if(_testone(map,x,y)==0)return 0;
	if(_testone(map,x-2,y+1)==0)return 0;
	if(_testone(map,x-1,y+1)==0)return 0;
	if(_testone(map,x,y+1)==0)return 0;
	if(_testone(map,x+1,y+1)==0)return 0;
	if(_testone(map,x+2,y+1)==0)return 0;
	if(_testone(map,x,y+2)==0)return 0;
	if(_testone(map,x-1,y+3)==0)return 0;
	if(_testone(map,x,y+3)==0)return 0;
	if(_testone(map,x+1,y+3)==0)return 0;
	
	map[x][y]=color;
	map[x-2][y+1]=color;
	map[x-1][y+1]=color;
	map[x][y+1]=color;
	map[x+1][y+1]=color;
	map[x+2][y+1]=color;
	map[x][y+2]=color;
	map[x-1][y+3]=color;
	map[x][y+3]=color;
	map[x+1][y+3]=color;
	return 1;
}
int _test4(int **map,int x,int y,int color){
	if(_testone(map,x,y)==0)return 0;
	if(_testone(map,x-2,y-1)==0)return 0;
	if(_testone(map,x-1,y-1)==0)return 0;
	if(_testone(map,x,y-1)==0)return 0;
	if(_testone(map,x+1,y-1)==0)return 0;
	if(_testone(map,x+2,y-1)==0)return 0;
	if(_testone(map,x,y-2)==0)return 0;
	if(_testone(map,x-1,y-3)==0)return 0;
	if(_testone(map,x,y-3)==0)return 0;
	if(_testone(map,x+1,y-3)==0)return 0;
	
	map[x][y]=color;
	map[x-2][y-1]=color;
	map[x-1][y-1]=color;
	map[x][y-1]=color;
	map[x+1][y-1]=color;
	map[x+2][y-1]=color;
	map[x][y-2]=color;
	map[x-1][y-3]=color;
	map[x][y-3]=color;
	map[x+1][y-3]=color;
	return 1;
}
int _test(int **map,int x,int y,int color){
	int mode,n=0;
	int flag[4]={0};
	while(1){
		if(n==3)break;
		mode=rand()%4;
		if(flag[mode]==0){
			flag[mode]=1;
			n++;
		}
		else{
			continue;
		}
		if(mode==0){
			if(_test1(map,x,y,color)==1){
				return mode+1;
			}
		}
		else if(mode==1){
			if(_test2(map,x,y,color)==1){
				return mode+1;
			}
		}
		else if(mode==2){
			if(_test3(map,x,y,color)==1){
				return mode+1;
			}
		}
		else{
			if(test4(map,x,y,color)==1){
				return mode+1;
			}
		}
	}
	return 0;
}
void Game::Game_init(){
	for(int i=0;i<MAP_SIZE;++i){
		for(int j=0;j<MAP_SIZE;++j){
			map[i][j]=0;
		}
	}
	srand((int)time(0));
	int n=0;
	while(1){
		if(n==3)break;
		int x1,y1;
		//飞机头部坐标
		x1=rand()%10;
		y1=rand()%10;
		int mode=_test(map,x1,y1,n+1);
		if(mode==1){
			plane[n].mode=mode;
			plane[n].head_x=x1;
			plane[n].head_y=y1;
			plane[n].tail_x=x1+3;
			plane[n].tail_y=y1;
			n++;
		}
		else if(mode==2){
			plane[n].mode=mode;
			plane[n].head_x=x1;
			plane[n].head_y=y1;
			plane[n].tail_x=x1-3;
			plane[n].tail_y=y1;
			n++;
		}
		else if(mode==3){
			plane[n].mode=mode;
			plane[n].head_x=x1;
			plane[n].head_y=y1;
			plane[n].tail_x=x1;
			plane[n].tail_y=y1+3;
			n++;
		}
		else if(mode==4){
			plane[n].mode=mode;
			plane[n].head_x=x1;
			plane[n].head_y=y1;
			plane[n].tail_x=x1;
			plane[n].tail_y=y1-3;
			n++;
		}
	}
	
}

void Game::Show_map(){
	for(int i=0;i<MAP_SIZE;++i){
		for(int j=0;j<MAP_SIZE;++j){
			cout<<map[i][j]<<" ";
		}
		cout<<endl;
	}
}