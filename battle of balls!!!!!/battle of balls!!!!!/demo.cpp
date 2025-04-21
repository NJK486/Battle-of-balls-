#include<stdio.h>
#include<easyx.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#define food_num 200
#define Width 1024
#define Height 640
#define Heifgt 10
#define Ai_num 10

struct Ball {
	int x;
	int y;
	int r;
	DWORD color;
	bool flag;
};
struct Ball food[food_num];
struct Ball player;
struct Ball ai[Ai_num];

void GameInit() {//初始化游戏
	//设置随机数种子
	srand((unsigned)time(NULL));
	//初始化食物
	for (int i = 0; i < food_num; i++) {
		food[i].x = rand() % Width;
		food[i].y = rand() % Height;//随机坐标
		food[i].flag = true;//食物存在
		food[i].r = 4;
		food[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);//随机颜色
	}
	//初始化ai
	for (int i = 0; i < Ai_num; i++) {
		ai[i].x = rand() % Width;
		ai[i].y = rand() % Height;//随机坐标
		ai[i].flag = true;//食物存在
		ai[i].r = 10;
		ai[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);//随机颜色(后续会上图片）
	}
	//初始化玩家
		player.x = 512;
		player.y = 320;//设定坐标
		player.flag = true;//食物存在
		player.r =10;
		player.color = RGB(rand() % 256, rand() % 256, rand() % 256);//随机颜色(后续会上图片）
}
void Gamedraw() {//绘制游戏背景
	BeginBatchDraw();
	setbkcolor(WHITE);//背景先设为白色(后续会改为透明图）
	cleardevice();
	for (int i = 0; i < food_num; i++) {
		if (food[i].flag) {
			setfillcolor(food[i].color);
			solidcircle(food[i].x, food[i].y, food[i].r);//画出食物
		}
		else//被吃掉后重新刷新
		{
			food[i].x = rand() % Width;
			food[i].y = rand() % Height;//随机坐标
			food[i].flag = true;//食物存在
			food[i].r = rand() % 6 + 1;
			food[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);//随机颜色
		}
	}
	//绘制玩家
	setfillcolor(player.color);
	solidcircle(player.x, player.y, player.r);
	settextcolor(GREEN);//设置文字颜色
	setbkmode(0);//文字背景透明
	settextstyle(30, 0, L"黑体");//设置字的样式
	outtextxy(player.x - 50, player.y - 50, L"长期素食");
	EndBatchDraw();
	//绘制ai
	for (int i = 0; i < Ai_num; i++) {
		if (ai[i].flag) {
			setfillcolor(ai[i].color);
			solidcircle(ai[i].x, ai[i].y, ai[i].r);//画出ai
		}
	}
}

int main() 
{
	initgraph(Width, Height);
	GameInit();
	while (true) {
		Gamedraw();
	}
	getchar();
	return 0;
}
