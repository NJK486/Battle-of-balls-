#include<easyx.h>
#include<time.h>
#include<math.h>
#include<conio.h>
#include"button.h"//按钮类
#include"scene.h"//场景类

#define PI 3.14//设置pai值
#define food_num 600//食物数量
#define Width 2000//整体地图宽度
#define Height 2000//整体地图高度
#define Ai_num 10//ai数量
#define SPIT_BALL_NUM 1000//最多吐球数量
#define SPIT_BALL_RADIUS 10.0  // 吐出球半径
#define max_split 2//最大可分裂球数

// 视图窗口大小
#define VIEW_WIDTH 1200//视窗宽度
#define VIEW_HEIGHT 900//视窗高度
int num = 1;//定义player初始球数

struct Ball {
    double x;  // 球的x坐标
    double y;  // 球的y坐标
    double r;  // 球的半径
    DWORD color;  // 球的颜色
    bool flag;  // 球的存活状态
};
struct Ball2 {
    double x;  // 球的x坐标
    double y;  // 球的y坐标
    double r;  // 球的半径
    DWORD color;  // 球的颜色
    bool flag;  // 球的存活状态
    IMAGE img;  // 球的图片
    IMAGE img2;//图片的掩码图
};
struct Ball3 {
    double x;  // 球的x坐标
    double y;  // 球的y坐标
    double r;  // 球的半径
    bool flag;  // 球的存活状态
    IMAGE img;  // 球的图片
    IMAGE img2;//图片的掩码图
};

struct Ball food[food_num];//食物结构体
struct Ball2 player[max_split];//玩家结构体  
struct Ball3 ai[Ai_num];//ai结构体
struct Ball spitBalls[max_split][SPIT_BALL_NUM];//吐出球结构体

// 视图偏移量
struct Offset {
    double x;//水平偏移量
    double y;//竖直偏移量
} offset;

POINT center;//中心点

void GameInit() {//游戏数据初始化
    srand((unsigned)time(NULL));//随机数种子
    //初始化食物
    for (int i = 0; i < food_num; i++) {
        food[i].x = rand() % Width;
        food[i].y = rand() % Height;//随机生成位置
        food[i].flag = true;
        food[i].r = 4;
        food[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);//随机颜色
    }
    for (int i = 0; i < Ai_num; i++) {
        ai[i].x = rand() % Width;
        ai[i].y = rand() % Height;//随机生成位置
        ai[i].flag = true;
        ai[i].r = 20;
        loadimage(&ai[i].img, _T("picture\\aiyin.jpg"), ai[i].r * 2, ai[i].r * 2, 1);
        loadimage(&ai[i].img2, _T("picture\\aiyin2.jpg"), ai[i].r * 2, ai[i].r * 2, 1);
    }
    player[0].x = rand() % Width;
    player[0].y = rand() % Height;  // 随机玩家球位置
    player[0].flag = true;
    player[0].r = 20;
    player[0].color = RGB(rand() % 256, rand() % 256, rand() % 256);  // 随机颜色
    // 加载玩家球的初始图片
    loadimage(&player[0].img, _T("picture\\player.jpg"), player[0].r * 2, player[0].r * 2, 1);
    loadimage(&player[0].img2, _T("picture\\player2.jpg"), player[0].r * 2, player[0].r * 2, 1);
    for (int i = 1; i < max_split; i++) {
        player[i].flag = false;  // 初始其他玩家球为未激活状态
    }

    // 初始化偏移量（使玩家位于视野中心）
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
    BeginBatchDraw();//避免出现闪屏
    setbkcolor(BLACK);
    cleardevice();
    // 绘制食物
    for (int i = 0; i < food_num; i++) {
        if (food[i].flag) {
            double drawX = food[i].x - offset.x;
            double drawY = food[i].y - offset.y;//食物整体坐标-偏移量=画图坐标
            if (drawX >= 0 && drawX < VIEW_WIDTH && drawY >= 0 && drawY < VIEW_HEIGHT) {//边界检查（留出一个像素画边界）
                setfillcolor(food[i].color);
                solidcircle(drawX, drawY, food[i].r);
            }
        }
        else {//当食物被吃掉时，重新初始化食物
            food[i].x = rand() % Width;
            food[i].y = rand() % Height;
            food[i].flag = true;
            food[i].r = 4;
            food[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
        }
    }
    // 绘制玩家
    for (int i = 0; i < max_split; i++) {
        if (player[i].flag) {
            double playerDrawX = player[i].x - offset.x;
            double playerDrawY = player[i].y - offset.y;

            // 重新加载并缩放图片以适应球的大小
            loadimage(&player[i].img, _T("picture\\player.jpg"), player[i].r * 2, player[i].r * 2, 1);
            loadimage(&player[i].img2, _T("picture\\player2.jpg"), player[i].r * 2, player[i].r * 2, 1);
            putimage(playerDrawX - player[i].r, playerDrawY - player[i].r, &player[i].img2, NOTSRCERASE);
            putimage(playerDrawX - player[i].r, playerDrawY - player[i].r, &player[i].img, SRCINVERT);

            settextcolor(WHITE);
            setbkmode(0);
            settextstyle(30, 0, L"微软雅黑");
            outtextxy(playerDrawX - 40, playerDrawY - player[i].r - 30, L"nagisaki");  // 在玩家球上方显示玩家名称
        }
    }
    // 绘制AI
    for (int i = 0; i < Ai_num; i++) {
        if (ai[i].flag) {
            double drawX = ai[i].x - offset.x;
            double drawY = ai[i].y - offset.y;
            if (drawX >= 0 && drawX < VIEW_WIDTH && drawY >= 0 && drawY < VIEW_HEIGHT) {//边界检查（留出一个像素画边界）
                loadimage(&ai[i].img, _T("picture\\aiyin.jpg"), ai[i].r * 2, ai[i].r * 2, 1);
                loadimage(&ai[i].img2, _T("picture\\aiyin2.jpg"), ai[i].r * 2, ai[i].r * 2, 1);
                putimage(drawX - ai[i].r, drawY - ai[i].r, &ai[i].img2, NOTSRCERASE);
                putimage(drawX - ai[i].r, drawY - ai[i].r, &ai[i].img, SRCINVERT);
            }
        }
    }
    //绘制吐出球
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


//操纵部分
struct mouse {//定义鼠标位置坐标结构体
    int x;
    int y;
}M;

void split(double x, double y, int i);//声明分裂，吐球函数
void spit(int i);
int dir;//方向变量

// 控制函数
void Control() {
    ExMessage msg;//信息结构体
    while (peekmessage(&msg)) {//接受信息
        if (msg.message == WM_MOUSEMOVE) {//鼠标移动信息
            M.x = msg.x;
            M.y = msg.y;
        }
        else if (msg.message == WM_KEYDOWN) {//键盘信息
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

struct next_position {//下一个位置
    double x;
    double y;
};
next_position N[max_split];//共有max_split个球，故用数组

void changeDir(double x, double y) {//改变方向函数
    int dX = M.x - (x - offset.x);
    int dY = M.y - (y - offset.y);//计算鼠标在视野面中的位置
    dir = atan2(dY, dX);//计算角度
}

const double SPEED = 1.9;//设置速度
double getspeed(double speed, double r) {//改变速度，半径越大，速度越慢
    double current_speed = speed * (100 / (100 + r));
    return current_speed;//返回当前速度
}

bool canMove() {//判定是否可以移动  
    for (int i = 0; i < max_split; i++) {
        if (player[i].flag) {//对于目前存在的所有球
            if (player[1].flag) {//判定如果有分裂球存在
                center.x = (player[0].x + player[1].x) / 2;
                center.y = (player[0].y + player[1].y) / 2;//计算两球之间中心点的坐标             
            }
            if (!player[1].flag) {//若无分裂球存在
                center.x = player[0].x;
                center.y = player[0].y;//定义中心点坐标为母球圆心坐标
            }
            if (!player[0].flag) {//若无分裂球存在
                center.x = player[1].x;
                center.y = player[1].y;//定义中心点坐标为母球圆心坐标
            }
            changeDir(center.x, center.y);//通过中心点坐标来计算方向角度
            N[i].x = player[i].x + (cos(dir) * getspeed(SPEED, player[i].r));
            N[i].y = player[i].y + (sin(dir) * getspeed(SPEED, player[i].r));//根据算好的角度计算每个球下一步的位置
            //边界处理效果（碰到边界反弹）
            if (N[i].x < player[i].r) {
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
            //对下一步的位置进行判定判定
            if (N[i].x < 0 + player[i].r || N[i].x > Width - player[i].r || N[i].y < 0 + player[i].r || N[i].y > Height - player[i].r) {
                return false;//如果碰到边界则返回false
            }
        }
    }
    return true;//经过所有的判定后返回true
}

void doMove() { // 玩家移动   
    for (int i = 0; i < max_split; i++) {
        if (player[i].flag) {
            player[i].x = N[i].x;
            player[i].y = N[i].y;//通过将下一步的坐标赋值给player实现移动
        }
    }
    // 更新偏移量（使玩家中心点始终处于视野中心）
    if (center.x - offset.x < VIEW_WIDTH / 2 - 1) {
        offset.x -= getspeed(SPEED, player[0].r);//依据玩家当前的速度，使视野随玩家移动
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

    // 边界检查（如果视野到达边界就固定不动）
    if (offset.x < 0) offset.x = 0;
    if (offset.y < 0) offset.y = 0;
    if (offset.x > Width - VIEW_WIDTH) offset.x = Width - VIEW_WIDTH;
    if (offset.y > Height - VIEW_HEIGHT) offset.y = Height - VIEW_HEIGHT;
}

//玩家移动函数总封装
void move() {
    if (canMove()) {
        doMove();
    }
}

// 分裂函数
void split(double x, double y, int i) {
    int active_balls = 0;
    for (int k = 0; k < max_split; k++) {
        if (player[k].flag) {
            active_balls++;//记录活跃球的数量
        }
    }
    if (active_balls < max_split && player[i].r > 40) { // 最多max_split个球且半径大于40才能分裂
        for (int j = 0; j < max_split; j++) {
            if (j != i && !player[j].flag) {
                player[j].flag = true;
                // 初始化分裂球的初始位置，与原球方向相同
                player[j].x = player[i].x + cos(dir) * (player[i].r + 50);
                player[j].y = player[i].y + sin(dir) * (player[i].r + 50);
                player[j].r = player[i].r / 2;
                player[j].color = player[i].color;
                player[i].r /= 2;//原来球的半径除二
                break;
            }
        }
    }
}

// 吐球函数
void spit(int i) {
    if (player[i].r > 30) { // 玩家半径大于30才能吐球
        for (int j = 0; j < SPIT_BALL_NUM; j++) {
            if (!spitBalls[i][j].flag) {
                // 初始化吐球的初始位置，与原球方向相同 
                spitBalls[i][j].x = player[i].x + cos(dir) * (player[i].r + 50);//定义吐出的球与原来球的距离
                spitBalls[i][j].y = player[i].y + sin(dir) * (player[i].r + 50);
                //吐出球的边界鉴定
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

//吃食物部分

double getDistance(double x1, double x2, double y1, double y2) {//计算距离的函数
    double distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    return distance;
}


double V(double r) {//计算球体积的函数
    double V = (4 * PI * pow(r, 3)) / 3;
    return V;
}

// 玩家吃球
// 1、体积小于即可吃
// 2、大球覆盖小球直径的75%判定吃入
// 3、吃入小球后大球半径增长小球半径的一半
// 4、吃入食物，半径固定增大0.5
// 5、吃入吐球，半径固定增大吐出球的半径
void player_eat() {
    for (int j = 0; j < max_split; j++) {
        if (player[j].flag) {
            for (int i = 0; i < food_num; i++) { // 吃食物
                if (getDistance(food[i].x, player[j].x, food[i].y, player[j].y) <= player[j].r + food[i].r) {
                    food[i].flag = 0;
                    player[j].r += 0.5;
                }
            }
            for (int i = 0; i < Ai_num; i++) { // 吃AI
                if (getDistance(ai[i].x, player[j].x, ai[i].y, player[j].y) - player[j].r <= ai[i].r / 4 && V(ai[i].r) <= V(player[j].r) && ai[i].flag == 1) {
                    ai[i].flag = 0;
                    player[j].r += ai[i].r / 2;
                }
            }

            // 玩家吃吐球
            for (int k = 0; k < max_split; k++) {
                for (int i = 0; i < SPIT_BALL_NUM; i++) {
                    if (spitBalls[k][i].flag && getDistance(spitBalls[k][i].x, player[j].x, spitBalls[k][i].y, player[j].y) - player[j].r <= spitBalls[k][i].r / 4) {
                        spitBalls[k][i].flag = false;
                        player[j].r += spitBalls[k][i].r;
                    }
                }
            }

            // 玩家球之间的互吃（合并机制）
            for (int i = 0; i < max_split; i++) {
                if (player[i].flag && i != j) {
                    if (V(player[i].r) < V(player[j].r) && getDistance(player[i].x, player[j].x, player[i].y, player[j].y) - player[j].r <= player[i].r / 4) {
                        player[i].flag = false;
                        player[j].r += player[i].r;
                    }
                }
            }
        }
    }
}

//ai部分
const double ai_speed = 0.3;//定义ai的速度
int Dir;//ai移动方向
//ai移动函数（只需知道目标点坐标即可移动）原理同玩家移动
void aimove(double x, double y, int i) {
    if (ai[i].flag == 1) {
        double dx = x - ai[i].x;
        double dy = y - ai[i].y;
        Dir = atan2(dy, dx);
        double newX = ai[i].x + (cos(Dir) * getspeed(SPEED, ai[i].r));
        double newY = ai[i].y + (sin(Dir) * getspeed(SPEED, ai[i].r));
        // 边界检查
        bool hit = 0;
        if (newX < ai[i].r) {
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
        if (hit == 1) {//如果碰壁则随机传送
            newX = rand() % Width;
            newY = rand() % Height;
        }
        if (newX >= 0 + ai[i].r && newX < Width - ai[i].r && newY >= 0 + ai[i].r && newY < Height - ai[i].r) {//进行边界判定
            ai[i].x = newX;
            ai[i].y = newY;
        }
    }
}

//ai吃物（同上述玩家吃物）
void ai_eat(int i) {
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
            if (k != i && ai[k].flag == 1 && getDistance(ai[k].x, ai[i].x, ai[k].y, ai[i].y) - ai[i].r <= ai[k].r / 4 && V(ai[k].r) < V(ai[i].r)) {
                ai[k].flag = 0;
            }
        }
        //ai吃玩家
        for (int j = 0; j < max_split; j++) {
            if (player[j].flag && getDistance(player[j].x, ai[i].x, player[j].y, ai[i].y) - ai[i].r <= player[j].r / 4 && V(player[j].r) < V(ai[i].r)) {
                player[j].flag = 0;
            }
        }
        //ai吃吐出球
        for (int k = 0; k < min(num, max_split); k++) {
            for (int j = 0; j < SPIT_BALL_NUM; j++) {
                if (k < num && j < SPIT_BALL_NUM && spitBalls[k][j].flag) {
                    if (spitBalls[k][j].flag && getDistance(spitBalls[k][j].x, ai[i].x, spitBalls[k][j].y, ai[i].y) - ai[i].r <= spitBalls[k][j].r / 4) {
                        spitBalls[k][j].flag = false;
                        ai[i].r += spitBalls[k][j].r;
                    }
                }
            }
        }
    }
}

//寻找最近的食物
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

//寻找玩家并吃掉
void findNearestPrey_player(int i, double& prey_x, double& prey_y) {
    double min = 1e9;
    for (int j = 0; j < num; j++) {
        if (player[j].flag == 1 && player[j].r < ai[i].r) {
            double dist = getDistance(ai[i].x, player[j].x, ai[i].y, player[j].y);
            if (dist < min) {
                min = dist;
                prey_x = player[j].x;
                prey_y = player[j].y;
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
        if ((j != i && ai[j].flag == 1 && ai[j].r > ai[i].r)) {
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

//规避玩家的攻击
void findNearestThreat_player(int i, double& threat_x, double& threat_y) {
    double min = 1e9;
    for (int j = 0; j < num; j++) {
        if (player[j].flag == 1 && player[j].r > ai[i].r) {
            double dist = getDistance(ai[i].x, player[j].x, ai[i].y, player[j].y);
            if (dist < min) {
                min = dist;
                threat_x = player[j].x;
                threat_y = player[j].y;
            }
        }
    }
    if (min == 1e9) {
        threat_x = -1;
        threat_y = -1;
    }
}

void aiplay() { // ai行为逻辑,先躲避，再攻击
    for (int i = 0; i < Ai_num; i++) {
        if (ai[i].flag == 1) {
            double target_x, target_y;//ai的目标位置
            double threat_x=0, threat_y=0;
            double threat_x1, threat_y1;//ai最近的威胁（AI）
            double threat_x2, threat_y2;//ai最近的威胁（玩家）
            // 寻找最近的威胁（AI）
            findNearestThreat(i, threat_x1, threat_y1);
            // 寻找最近的威胁（玩家）
            findNearestThreat_player(i, threat_x2, threat_y2);

            // 选择最近的威胁
            if (getDistance(ai[i].x, threat_x1, ai[i].y, threat_y1) < getDistance(ai[i].x, threat_x2, ai[i].y, threat_y2)) {
                threat_x = threat_x1;
                threat_y = threat_y1;
            }
            if (getDistance(ai[i].x, threat_x1, ai[i].y, threat_y1) >= getDistance(ai[i].x, threat_x2, ai[i].y, threat_y2)) {
                threat_x = threat_x2;
                threat_y = threat_y2;
            }

            if (getDistance(ai[i].x, threat_x, ai[i].y, threat_y) < 50) {//距离小于50开始逃窜
                double dx = ai[i].x - threat_x;
                double dy = ai[i].y - threat_y;//反方向走
                target_x = ai[i].x + dx;
                target_y = ai[i].y + dy;
            }
            else {//在确定周围无危险后
                bool foundPrey = false;
                // 寻找最近的猎物（AI）
                double prey_x1, prey_y1;
                findNearestPrey(i, prey_x1, prey_y1);
                // 寻找最近的猎物（玩家）
                double prey_x2, prey_y2;
                findNearestPrey_player(i, prey_x2, prey_y2);

                // 选择最近的猎物
                if (getDistance(ai[i].x, prey_x1, ai[i].y, prey_y1) < getDistance(ai[i].x, prey_x2, ai[i].y, prey_y2)) {
                    target_x = prey_x1;
                    target_y = prey_y1;
                    foundPrey = true;
                }
                if (getDistance(ai[i].x, prey_x1, ai[i].y, prey_y1) >= getDistance(ai[i].x, prey_x2, ai[i].y, prey_y2)) {
                    target_x = prey_x2;
                    target_y = prey_y2;
                    foundPrey = true;
                }
                else if (prey_x1 == -1) {
                    target_x = prey_x2;
                    target_y = prey_y2;
                    foundPrey = true;
                }
                if (!foundPrey) {
                    // 没有猎物，寻找最近的食物
                    findNearestFood(i, target_x, target_y);
                }
            }
            aimove(target_x, target_y, i);
        }
        ai_eat(i);
    }
}

//整体界面方面
void Start();//显示开始界面
void showend();//显示结束界面
void game_playing();//游戏主体
void showESC();//显示暂停界面
void victory();//显示胜利界面

bool isPaused = 0;//定义暂停状态

void game_playing() {//游戏主进程
    bool gameover = 0;
    while (!gameover) {
        if (_kbhit()) {
            if (_getch() == 27) {//按下ESC暂停
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
        bool die = 1;//定义玩家死亡状态
        for (int i = 0; i < max_split; i++) {
            if (player[i].flag) {
                die = 0;//只要有一个玩家存活就判定未死亡
            }
        }
        if (die) {
            gameover = 1;
        }
        int count = 0; // 统计死亡的 AI 数量
        for (int i = 0; i < Ai_num; i++) {
            if (!ai[i].flag) {
                count++;
            }
        }
        if (count == Ai_num && !die) { // 所有 AI 死亡且玩家存活，判定胜利
            victory();
            gameover = true;
        }
    }
}

void Start() {
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
    Sleep(10);
}

void showend() {//显示结束界面
    scene s2{ {0,0},{VIEW_WIDTH,VIEW_HEIGHT} };
    s2.show_end();

    POINT btnPos = { VIEW_WIDTH / 2 - 300, VIEW_HEIGHT / 2 + 140 };
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
        };
    ExMessage msg;
    while (!gameReturn || !gameend) {
        returnButton.Draw_return();
        endButton.Draw_end();
        while (peekmessage(&msg)) {
            returnButton.Event_Tick(&msg);
            endButton.Event_Tick(&msg);
        }
    }
    Sleep(10);
}

void victory() {//显示胜利画面
    scene s{ {0,0},{VIEW_WIDTH,VIEW_HEIGHT} };
    s.victory();
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
        };
    ExMessage msg;
    while (!gameReturn || !gameend) {
        returnButton.Draw_return();
        endButton.Draw_end();
        while (peekmessage(&msg)) {
            returnButton.Event_Tick(&msg);
            endButton.Event_Tick(&msg);
        }
    }
    Sleep(10);
}

//主程序
int main() {
    Start();
    GameInit();
    game_playing();
    showend();
}