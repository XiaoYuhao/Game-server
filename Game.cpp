#include<iostream>
#include<stdlib.h>
#include<time.h>
#include"Game.h"

int test1(int x,int y){
	int x1,y1;
	x1=x+1;
	y1=y-2;
	if(x1<0||x1>9||y1<0||y1>9){
		return 0;
	}
	int x2,y2;
	x2=x+1;
	y2=y+2;
	if(x2<0||x2>9||y2<0||y2>9){
		return 0;
	}
	int x3,y3;
	x3=x+3;
	y3=y-1;
	if(x3<0||x3>9||y3<0||y3>9){
		return 0;
	}
	int x4,y4;
	x4=x+3;
	y4=y+1;
	if(x4<0||x4>9||y4<0||y4>9){
		return 0;
	}
	return 1;
}
int test2(int x,int y){
	int x1,y1;
	x1=x-1;
	y1=y-2;
	if(x1<0||x1>9||y1<0||y1>9){
		return 0;
	}
	int x2,y2;
	x2=x-1;
	y2=y+2;
	if(x2<0||x2>9||y2<0||y2>9){
		return 0;
	}
	int x3,y3;
	x3=x-3;
	y3=y-1;
	if(x3<0||x3>9||y3<0||y3>9){
		return 0;
	}
	int x4,y4;
	x4=x-3;
	y4=y+1;
	if(x4<0||x4>9||y4<0||y4>9){
		return 0;
	}
	return 1;
}
int test3(int x,int y){
	int x1,y1;
	x1=x+2;
	y1=y+1;
	if(x1<0||x1>9||y1<0||y1>9){
		return 0;
	}
	int x2,y2;
	x2=x-2;
	y2=y+1;
	if(x2<0||x2>9||y2<0||y2>9){
		return 0;
	}
	int x3,y3;
	x3=x+1;
	y3=y+3;
	if(x3<0||x3>9||y3<0||y3>9){
		return 0;
	}
	int x4,y4;
	x4=x-1;
	y4=y+3;
	if(x4<0||x4>9||y4<0||y4>9){
		return 0;
	}
	return 1;
}
int test4(int x,int y){
	int x1,y1;
	x1=x-2;
	y1=y-1;
	if(x1<0||x1>9||y1<0||y1>9){
		return 0;
	}
	int x2,y2;
	x2=x+2;
	y2=y-1;
	if(x2<0||x2>9||y2<0||y2>9){
		return 0;
	}
	int x3,y3;
	x3=x+1;
	y3=y-3;
	if(x3<0||x3>9||y3<0||y3>9){
		return 0;
	}
	int x4,y4;
	x4=x-1;
	y4=y-3;
	if(x4<0||x4>9||y4<0||y4>9){
		return 0;
	}
	return 1;
}
int test(int x,int y){
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
			if(test1(x,y)==1){
				return mode+1;
			}
		}
		else if(mode==1){
			if(test2(x,y)==1){
				return mode+1;
			}
		}
		else if(mode==2){
			if(test3(x,y)==1){
				return mode+1;
			}
		}
		else{
			if(test4(x,y)==1){
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
	int x1,y1,x2,y2;
	//飞机头部坐标
	x1=rand()%10;
	y1=rand()%10;
	
	
}