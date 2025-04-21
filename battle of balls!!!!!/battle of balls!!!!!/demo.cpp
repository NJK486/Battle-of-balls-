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

void GameInit() {//��ʼ����Ϸ
	//�������������
	srand((unsigned)time(NULL));
	//��ʼ��ʳ��
	for (int i = 0; i < food_num; i++) {
		food[i].x = rand() % Width;
		food[i].y = rand() % Height;//�������
		food[i].flag = true;//ʳ�����
		food[i].r = 4;
		food[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);//�����ɫ
	}
	//��ʼ��ai
	for (int i = 0; i < Ai_num; i++) {
		ai[i].x = rand() % Width;
		ai[i].y = rand() % Height;//�������
		ai[i].flag = true;//ʳ�����
		ai[i].r = 10;
		ai[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);//�����ɫ(��������ͼƬ��
	}
	//��ʼ�����
		player.x = 512;
		player.y = 320;//�趨����
		player.flag = true;//ʳ�����
		player.r =10;
		player.color = RGB(rand() % 256, rand() % 256, rand() % 256);//�����ɫ(��������ͼƬ��
}
void Gamedraw() {//������Ϸ����
	BeginBatchDraw();
	setbkcolor(WHITE);//��������Ϊ��ɫ(�������Ϊ͸��ͼ��
	cleardevice();
	for (int i = 0; i < food_num; i++) {
		if (food[i].flag) {
			setfillcolor(food[i].color);
			solidcircle(food[i].x, food[i].y, food[i].r);//����ʳ��
		}
		else//���Ե�������ˢ��
		{
			food[i].x = rand() % Width;
			food[i].y = rand() % Height;//�������
			food[i].flag = true;//ʳ�����
			food[i].r = rand() % 6 + 1;
			food[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);//�����ɫ
		}
	}
	//�������
	setfillcolor(player.color);
	solidcircle(player.x, player.y, player.r);
	settextcolor(GREEN);//����������ɫ
	setbkmode(0);//���ֱ���͸��
	settextstyle(30, 0, L"����");//�����ֵ���ʽ
	outtextxy(player.x - 50, player.y - 50, L"������ʳ");
	EndBatchDraw();
	//����ai
	for (int i = 0; i < Ai_num; i++) {
		if (ai[i].flag) {
			setfillcolor(ai[i].color);
			solidcircle(ai[i].x, ai[i].y, ai[i].r);//����ai
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
