#include<stdio.h>
#include<easyx.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<conio.h>
#include"button.h"
#include"scene.h"
#include<iostream>
using namespace std;

//获取unicode字符串
#ifdef UNICODE
#undef UNICODE
#endif

#define food_num 600
#define Width 2000
#define Height 2000
#define Ai_num 30
#define spit_num 1000
#define SPIT_BALL_RADIUS 10.0  // 吐出球半径
#define max_split 2

// 视图窗口大小
#define VIEW_WIDTH 1200
#define VIEW_HEIGHT 900
int num = 1;

struct Ball {
    double x;
    double y;
    double r;
    DWORD color;
    bool flag;
};
struct Ball food[food_num];
struct Ball player[max_split];  // 修改为数组结构体
struct Ball ai[Ai_num];

struct SpitBall {
    double x;
    double y;
    double r;
    DWORD color;
    bool flag;
};
#define SPIT_BALL_NUM 100
SpitBall spitBalls[max_split][SPIT_BALL_NUM];

// 视图偏移量
struct Offset {
    double x;
    double y;
} offset;
POINT center;

void GameInit() {
    srand((unsigned)time(NULL));
    for (int i = 0; i < food_num; i++) {
        food[i].x = rand() % Width;
        food[i].y = rand() % Height;
        food[i].flag = true;
        food[i].r = 4;
        food[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
    }
    for (int i = 0; i < Ai_num; i++) {
        ai[i].x = rand() % Width;
        ai[i].y = rand() % Height;
        ai[i].flag = true;
        ai[i].r = 20;
        ai[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
    }
    player[0].x = rand() % Width;
    player[0].y = rand() % Height;
    player[0].flag = true;
    player[0].r = 20;
    player[0].color = RGB(rand() % 256, rand() % 256, rand() % 256);
    for (int i = 1; i < max_split; i++) {
        player[i].flag = false;  // 初始化分裂球为未激活状态
    }

    // 初始化偏移量
    offset.x = player[0].x - VIEW_WIDTH / 2;
    offset.y = player[0].y - VIEW_HEIGHT / 2;

    // 初始化吐出球
    for (int j = 0; j < max_split; j++) {
        for (int i = 0; i < SPIT_BALL_NUM; i++) {
            spitBalls[j][i].flag = false;
        }
    }
}

void Gamedraw() {//绘制
    BeginBatchDraw();
    setbkcolor(BLACK);
    cleardevice();

    // 绘制食物
    for (int i = 0; i < food_num; i++) {
        if (food[i].flag) {
            double drawX = food[i].x - offset.x;
            double drawY = food[i].y - offset.y;
            if (drawX >= 0 && drawX < VIEW_WIDTH && drawY >= 0 && drawY < VIEW_HEIGHT) {
                setfillcolor(food[i].color);
                solidcircle(drawX, drawY, food[i].r);
            }
        }
        else {
            food[i].x = rand() % Width;
            food[i].y = rand() % Height;
            food[i].flag = true;
            food[i].r = rand() % 6 + 2;
            food[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
        }
    }

    // 绘制玩家
    for (int i = 0; i < max_split; i++) {
        if (player[i].flag) {
            double playerDrawX = player[i].x - offset.x;
            double playerDrawY = player[i].y - offset.y;
            setfillcolor(player[i].color);
            solidcircle(playerDrawX, playerDrawY, player[i].r);
            settextcolor(GREEN);
            setbkmode(0);
            settextstyle(30, 0, L"宋体");
            outtextxy(playerDrawX - 50, playerDrawY - 50, L"玩家吃物");
        }
    }

    // 绘制AI
    for (int i = 0; i < Ai_num; i++) {
        if (ai[i].flag) {
            double drawX = ai[i].x - offset.x;
            double drawY = ai[i].y - offset.y;
            if (drawX >= 0 && drawX < VIEW_WIDTH && drawY >= 0 && drawY < VIEW_HEIGHT) {
                setfillcolor(ai[i].color);
                solidcircle(drawX, drawY, ai[i].r);
            }
        }
    }

    for (int j = 0; j < max_split; j++) {
        if (player[j].flag) {
            for (int i = 0; i < SPIT_BALL_NUM; i++) {
                if (spitBalls[j][i].flag) {
                    double drawX = spitBalls[j][i].x - offset.x;
                    double drawY = spitBalls[j][i].y - offset.y;
                    if (drawX >= 0 && drawX < VIEW_WIDTH && drawY >= 0 && drawY < VIEW_HEIGHT) {
                        setfillcolor(player[j].color);
                        solidcircle(drawX, drawY, spitBalls[j][i].r);
                        setfillcolor(WHITE);
                        circle(drawX, drawY, spitBalls[j][i].r);
                    }
                }
            }
        }
    }
    EndBatchDraw();
}

struct mouse {
    int x;
    int y;
}M;

void split(double x, double y,int i);
void spit(int i);
int dir;

// 修改前
// void Control() {//获取玩家移动消息
// 修改后
void Control() {
    ExMessage msg;
    while (peekmessage(&msg)) {
        if (msg.message == WM_MOUSEMOVE) {
            M.x = msg.x;
            M.y = msg.y;
        }
        else if (msg.message == WM_KEYDOWN) {
            if (msg.vkcode == VK_SPACE) { // 按下空格键进行分裂
                for (int i = 0; i < max_split; i++) {
                    if (player[i].flag) {
                        split(player[i].x, player[i].y, i);
                    }
                }
            }
            else if (msg.vkcode == VK_CONTROL) { // 按下Ctrl键进行吐球
                for (int i = 0; i < max_split; i++) {
                    if (player[i].flag) {
                        spit(i);
                    }
                }
            }
        }
    }
}

struct next_position {
    double x;
    double y;
};
next_position N[2];

void changeDir(double x, double y) {
    int dX = M.x - (x - offset.x);
    int dY = M.y - (y - offset.y);
    dir = atan2(dY, dX);
}

const double SPEED = 0.3;//设置速度
double getspeed(double speed, double r) {//改变速度，半径越大，速度越慢
    double current_speed = speed * (100 / (100 + r));
    return current_speed;
}

bool canMove() {  
    for (int i = 0; i < 2; i++) {
        if (player[i].flag) {
            if (player[1].flag) {
                center.x = (player[0].x + player[1].x) / 2;
                center.y = (player[0].y + player[1].y) / 2;             
            }
            if (!player[1].flag) {
                center.x = player[0].x;
                center.y = player[0].y;
            } 
            if (!player[0].flag) {
                center.x = player[1].x;
                center.y = player[1].y;
            }
            changeDir(center.x, center.y);
            N[i].x = player[i].x + (cos(dir) * getspeed(SPEED, player[i].r));
            N[i].y = player[i].y + (sin(dir) * getspeed(SPEED, player[i].r));
            if (N[i].x < player[i].r) {//边界处理效果
                N[i].x = player[i].r;
            }
            else if (N[i].x > Width - player[i].r) {
                N[i].x = Width - player[i].r;
            }
            if (N[i].y < player[i].r) {
                N[i].y = player[i].r;
            }
            else if (N[i].y > Height - player[i].r) {
                N[i].y = Height - player[i].r;
            }
            if (N[i].x < 0 + player[i].r || N[i].x > Width - player[i].r || N[i].y < 0 + player[i].r || N[i].y > Height - player[i].r) {
                return false;
            }
        }
    }
    return true;
}

void doMove() {
    // 玩家移动
    for (int i = 0; i < max_split; i++) {
        if (player[i].flag) {
            player[i].x = N[i].x;
            player[i].y = N[i].y;
        }
    }

    // 更新偏移量
    if (center.x - offset.x < VIEW_WIDTH / 2 - 1) {
        offset.x -= getspeed(SPEED, player[0].r);
    }
    else if (center.x - offset.x > VIEW_WIDTH / 2 + 1) {
        offset.x += getspeed(SPEED, player[0].r);
    }
    if (center.y - offset.y < VIEW_HEIGHT / 2 - 1) {
        offset.y -= getspeed(SPEED, player[0].r);
    }
    else if (center.y - offset.y > VIEW_HEIGHT / 2 + 1) {
        offset.y += getspeed(SPEED, player[0].r);
    }

    // 边界检查
    if (offset.x < 0) offset.x = 0;
    if (offset.y < 0) offset.y = 0;
    if (offset.x > Width - VIEW_WIDTH) offset.x = Width - VIEW_WIDTH;
    if (offset.y > Height - VIEW_HEIGHT) offset.y = Height - VIEW_HEIGHT;
}

void move() {
    if (canMove()) {
        doMove();
    }
}

double getDistance(double x1, double x2, double y1, double y2) {
    double distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    return distance;
}

// 修改前
// void split(double x, double y,int i) {
// 修改后
void split(double x, double y, int i) {
    int active_balls = 0;
    for (int k = 0; k < max_split; k++) {
        if (player[k].flag) {
            active_balls++;
        }
    }
    if (active_balls < max_split && player[i].r > 40) { // 最多4个球且半径大于40才能分裂
        for (int j = 0; j < max_split; j++) {
            if (j != i && !player[j].flag) {
                // 初始化分裂球的初始位置，与原球方向相反 
                player[j].flag = true;
                player[j].x = player[i].x + cos(dir) * (player[i].r + 50);
                player[j].y = player[i].y + sin(dir) * (player[i].r + 50);
                player[j].r = player[i].r / 2;
                player[j].color = player[i].color;
                player[i].r /= 2;
                break;
            }
        }
    }
}

// 修改spit函数
void spit(int i) {
    if (player[i].r > 30) { // 玩家半径大于30才能吐球
        for (int j = 0; j < SPIT_BALL_NUM; j++) {
            if (!spitBalls[i][j].flag) {
                // 初始化吐球的初始位置，与原球方向相同 
                spitBalls[i][j].x = player[i].x + cos(dir) * (player[i].r + 50);
                spitBalls[i][j].y = player[i].y + sin(dir) * (player[i].r + 50);
                if (spitBalls[i][j].x < spitBalls[i][j].r) {
                    spitBalls[i][j].x = spitBalls[i][j].r;
                }
                else if (spitBalls[i][j].x > Width - spitBalls[i][j].r) {
                    spitBalls[i][j].x = Width - spitBalls[i][j].r;
                }
                if (spitBalls[i][j].y < spitBalls[i][j].r) {
                    spitBalls[i][j].y = spitBalls[i][j].r;
                }
                else if (spitBalls[i][j].y > Height - spitBalls[i][j].r) {
                    spitBalls[i][j].y = Height - spitBalls[i][j].r;
                }
                spitBalls[i][j].r = SPIT_BALL_RADIUS;
                spitBalls[i][j].color = player[i].color;
                spitBalls[i][j].flag = true;
                player[i].r -= SPIT_BALL_RADIUS;  // 减小玩家半径
                break;
            }
        }
    }
}


// 修改后
void player_eat() {
    for (int j = 0; j < max_split; j++) {
        if (player[j].flag) {
            for (int i = 0; i < food_num; i++) { // 吃食物
                if (getDistance(food[i].x, player[j].x, food[i].y, player[j].y) <= player[j].r + food[i].r) {
                    food[i].flag = 0;
                    food[i].color = BLACK;
                    player[j].r += 0.5;
                }
            }
            for (int i = 0; i < Ai_num; i++) { // 吃AI
                if (getDistance(ai[i].x, player[j].x, ai[i].y, player[j].y) <= player[j].r + ai[i].r && ai[i].r < player[j].r && ai[i].flag == 1) {
                    ai[i].flag = 0;
                    ai[i].color = BLACK;
                    player[j].r += ai[i].r / 2;
                }
            }

            // 玩家吃吐球
            for (int k = 0; k < max_split; k++) {
                for (int i = 0; i < SPIT_BALL_NUM; i++) {
                    if (spitBalls[k][i].flag && getDistance(spitBalls[k][i].x, player[j].x, spitBalls[k][i].y, player[j].y) <= player[j].r + spitBalls[k][i].r) {
                        spitBalls[k][i].flag = false;
                        player[j].r += spitBalls[k][i].r;
                    }
                }
            }

            // 玩家球之间的互吃
            for (int i = 0; i < max_split; i++) {
                if (player[i].flag && i != j) {
                    if (player[i].r < player[j].r + 10 && getDistance(player[i].x, player[j].x, player[i].y, player[j].y) <= (player[j].r + player[i].r) / 2) {
                        player[i].flag = false;
                        player[j].r += player[i].r;
                    }
                }
            }
        }
    }
}

const double ai_speed = 0.3;
int Dir;//ai移动方向
void aimove(double x, double y, int i) {//ai移动逻辑
    if (ai[i].flag == 1) {
        double dx = x - ai[i].x;
        double dy = y - ai[i].y;
        Dir = atan2(dy, dx);
        double newX = ai[i].x + (cos(Dir) * getspeed(SPEED, ai[i].r));
        double newY = ai[i].y + (sin(Dir) * getspeed(SPEED, ai[i].r));
        // 边界检查
        bool hit = 0;
        if (newX < ai[i].r) {//边界处理效果
            newX = ai[i].r;
            hit = 1;
        }
        else if (newX > Width - ai[i].r) {
            newX = Width - ai[i].r;
            hit = 1;
        }
        if (newY < ai[i].r) {
            newY = ai[i].r;
            hit = 1;
        }
        else if (newY > Height - ai[i].r) {
            newY = Height - ai[i].r;
            hit = 1;
        }
        if (hit == 1) {//如果碰壁则直接返回原地
            newX = 1000;
            newY = 1000;
        }
        if (newX >= 0 + ai[i].r && newX < Width - ai[i].r && newY >= 0 + ai[i].r && newY < Height - ai[i].r) {
            ai[i].x = newX;
            ai[i].y = newY;
        }
    }
}

void ai_eat(int i) {//ai吃物
    if (ai[i].flag == 1) {
        for (int j = 0; j < food_num; j++) {//ai吃食物
            if (getDistance(food[j].x, ai[i].x, food[j].y, ai[i].y) <= ai[i].r + food[j].r) {
                food[j].flag = 0;
                food[j].color = BLACK;
                ai[i].r += 0.5;
            }
        }
        //ai吃ai
        for (int k = 0; k < Ai_num; k++) {
            if (k != i && ai[k].flag == 1 && getDistance(ai[k].x, ai[i].x, ai[k].y, ai[i].y) <= (ai[i].r + ai[k].r)/2 && ai[k].r < ai[i].r) {
                ai[k].flag = 0;
                ai[k].color = BLACK;
                ai[i].r += ai[k].r / 2;
            }
        }
        //ai吃玩家
        for (int j = 0; j < max_split; j++) {
            if (player[j].flag && getDistance(player[j].x, ai[i].x, player[j].y, ai[i].y) <= (ai[i].r + player[j].r) / 2) {
                player[j].flag = 0;
            }
        }
        //ai吃吐出球
        for (int k = 0; k < min(num, max_split); k++) {
            for (int j = 0; j < SPIT_BALL_NUM; j++) {
                if (k < num && j < SPIT_BALL_NUM && spitBalls[k][j].flag) {
                    if (spitBalls[k][j].flag && getDistance(spitBalls[k][j].x, ai[i].x, spitBalls[k][j].y, ai[i].y) <= ai[i].r + spitBalls[k][j].r) {
                        spitBalls[k][j].flag = false;
                        ai[i].r += spitBalls[k][j].r;
                    }
                }
            }
        }
    }
}

void avoidCollision(int index) {//ai避免碰撞
    for (int i = 0; i < Ai_num; i++) {
        if (i != index && ai[i].flag == 1 && getDistance(ai[i].x, ai[index].x, ai[i].y, ai[index].y) <= ai[i].r + ai[index].r +5) {
            double dx = ai[index].x - ai[i].x;
            double dy = ai[index].y - ai[i].y;
            Dir = atan2(dy, dx);
            double newX = ai[index].x + (cos(Dir) * getspeed(SPEED, ai[i].r));
            double newY = ai[index].y + (sin(Dir) * getspeed(SPEED, ai[i].r));

            // 边界检查
            if (newX >= 0 && newX < Width && newY >= 0 && newY < Height) {
                ai[index].x = newX;
                ai[index].y = newY;
            }
        }
    }
}

void findNearestFood(int i, double& food_x, double& food_y) {
    double min = getDistance(ai[i].x, food[0].x, ai[i].y, food[0].y);
    food_x = food[0].x;
    food_y = food[0].y;
    for (int j = 1; j < food_num; j++) {
        double dist = getDistance(ai[i].x, food[j].x, ai[i].y, food[j].y);
        if (dist < min) {
            min = dist;
            food_x = food[j].x;
            food_y = food[j].y;
        }
    }
}

// 寻找最近的猎物AI
void findNearestPrey(int i, double& prey_x, double& prey_y) {
    double min = 1e9;
    for (int j = 0; j < Ai_num; j++) {
        if (j != i && ai[j].flag == 1 && ai[j].r < ai[i].r) {
            double dist = getDistance(ai[i].x, ai[j].x, ai[i].y, ai[j].y);
            if (dist < min) {
                min = dist;
                prey_x = ai[j].x;
                prey_y = ai[j].y;
            }
        }
    }
    if (min == 1e9) {
        prey_x = -1;
        prey_y = -1;
    }
}

// 寻找最近的威胁AI
void findNearestThreat(int i, double& threat_x, double& threat_y) {
    double min = 1e9;
    for (int j = 0; j < Ai_num; j++) {
        if (j != i && ai[j].flag == 1 && ai[j].r > ai[i].r) {
            double dist = getDistance(ai[i].x, ai[j].x, ai[i].y, ai[j].y);
            if (dist < min) {
                min = dist;
                threat_x = ai[j].x;
                threat_y = ai[j].y;
            }
        }
    }
    if (min == 1e9) {
        threat_x = -1;
        threat_y = -1;
    }
}

void aiplay() { // ai行为逻辑
    for (int i = 0; i < Ai_num; i++) {
        if (ai[i].flag == 1) {
            double target_x, target_y;
            double threat_x, threat_y;

            // 寻找最近的威胁AI
            findNearestThreat(i, threat_x, threat_y);
            if (threat_x != -1 && getDistance(ai[i].x, threat_x, ai[i].y, threat_y) < 100) {
                // 遇到威胁，逃跑
                double dx = ai[i].x - threat_x;
                double dy = ai[i].y - threat_y;
                target_x = ai[i].x + dx;
                target_y = ai[i].y + dy;
            }
            else {
                bool foundPrey = false;
                // 寻找最近的猎物AI
                double prey_x, prey_y;
                findNearestPrey(i, prey_x, prey_y);
                if (prey_x != -1) {
                    target_x = prey_x;
                    target_y = prey_y;
                    foundPrey = true;
                }
                if (!foundPrey) {
                    // 寻找最近的食物
                    findNearestFood(i, target_x, target_y);
                }
            }
            aimove(target_x, target_y, i);
        }
        ai_eat(i);
    }
}

void showend();//显示结束界面
void Start();
void game_playing();
void showESC();
bool isPaused = 0;

void game_playing() {//游戏主进程
    bool gameover = 0;
    while (!gameover) {
        if (_kbhit()) {
            if (_getch() == 27) {
                showESC();
                isPaused = 1;
            }
        }
        if (!isPaused) {
           Control();
           Gamedraw();
           move();
           player_eat();
           aiplay();
        }
        bool die = 1;
        for (int i = 0; i < max_split; i++) {
            if (player[i].flag ) {
                die = 0;
                break;
            }
        }
        if (die) {
            gameover = 1;
        }
    }
    showend();
}

void Start() {//显示开始界面 
    initgraph(VIEW_WIDTH, VIEW_HEIGHT);
    scene s1{ {0,0},{VIEW_WIDTH,VIEW_HEIGHT} };
    s1.draw_start();
    POINT btnPos = { VIEW_WIDTH / 2 - 300, VIEW_HEIGHT / 2 + 250 };
    SIZE btnSize = { 600, 140 };
    COLORREF btnColor = RGB(192, 192, 192);
    button startButton(btnPos, btnSize, btnColor);
    bool gameStarted = false;
    startButton.Onclick = [&gameStarted]() {
        gameStarted = true;
        game_playing();
        };

    ExMessage msg;
    while (!gameStarted) {
        startButton.Draw_start();
        while (peekmessage(&msg)) {
            startButton.Event_Tick(&msg);
        }
        Sleep(10);
    }
}
void showESC() {
    scene s3{ {0,0},{VIEW_WIDTH,VIEW_HEIGHT} };
    s3.showESC();
    POINT btnPos = { VIEW_WIDTH / 2 - 300, VIEW_HEIGHT / 2 + 150 };
    SIZE btnSize = { 600, 140 };
    COLORREF btnColor = RGB(192, 192, 192);
    button returnbutton(btnPos, btnSize, btnColor);
    returnbutton.Draw_return();
    returnbutton.Onclick = [] {
        isPaused = 0;
        game_playing();
    };
    //退出游戏健
    POINT btnPos1 = { VIEW_WIDTH / 2 - 300, VIEW_HEIGHT / 2 + 300 };
    SIZE btnSize1 = { 600, 140 };
    COLORREF btnColor1 = RGB(192, 192, 192);
    button startButton(btnPos1, btnSize1, btnColor1);
    startButton.Draw_end();
    bool gameend = false;
    startButton.Onclick = [&gameend] {
        gameend = 1;
        GameInit();
        Start();
        };
    ExMessage msg;
    while (!gameend || isPaused) {
        returnbutton.Draw_return();
        startButton.Draw_end();
        while (peekmessage(&msg)) {
            startButton.Event_Tick(&msg);
            returnbutton.Event_Tick(&msg);
        }
    }
}

void showend() {//显示结束界面
    scene s2{ {0,0},{VIEW_WIDTH,VIEW_HEIGHT} };
    s2.show_end();

    POINT btnPos = { VIEW_WIDTH / 2 - 300, VIEW_HEIGHT / 2 + 150 };
    SIZE btnSize = { 600, 140 };
    COLORREF btnColor = RGB(192, 192, 192);
    button returnButton(btnPos, btnSize, btnColor);
    returnButton.Draw_return();
    POINT btnPos1 = { VIEW_WIDTH / 2 - 300, VIEW_HEIGHT / 2 + 300 };
    SIZE btnSize1 = { 600, 140 };
    COLORREF btnColor1 = RGB(192, 192, 192);
    button endButton(btnPos1, btnSize1, btnColor1);
    endButton.Draw_end();
    bool gameReturn = false;
    returnButton.Onclick = [&gameReturn]() {
        gameReturn = true;
        // 重置游戏状态，重新开始
        GameInit();
        Start();
        };
    bool gameend = false;
    endButton.Onclick = [&gameend] {
        gameend = 1;
        closegraph();  // 关闭图形窗口
        exit(0);       // 正常退出程序
        return 0;
    };
    ExMessage msg;
    while (!gameReturn||!gameend) {
        returnButton.Draw_return();
        endButton.Draw_end();
        while (peekmessage(&msg)) {
            returnButton.Event_Tick(&msg);
            endButton.Event_Tick(&msg);
        }
        Sleep(10);
    }
}

int main() {
    Start();
    GameInit();
    game_playing();
    return 0;
}