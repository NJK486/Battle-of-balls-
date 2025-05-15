#include<easyx.h>
#include<time.h>
#include<math.h>
#include<conio.h>
#include"button.h"//��ť��
#include"scene.h"//������

#define PI 3.14//����paiֵ
#define food_num 600//ʳ������
#define Width 2000//�����ͼ���
#define Height 2000//�����ͼ�߶�
#define Ai_num 10//ai����
#define SPIT_BALL_NUM 1000//�����������
#define SPIT_BALL_RADIUS 10.0  // �³���뾶
#define max_split 2//���ɷ�������

// ��ͼ���ڴ�С
#define VIEW_WIDTH 1200//�Ӵ����
#define VIEW_HEIGHT 900//�Ӵ��߶�
int num = 1;//����player��ʼ����

struct Ball {
    double x;  // ���x����
    double y;  // ���y����
    double r;  // ��İ뾶
    DWORD color;  // �����ɫ
    bool flag;  // ��Ĵ��״̬
};
struct Ball2 {
    double x;  // ���x����
    double y;  // ���y����
    double r;  // ��İ뾶
    DWORD color;  // �����ɫ
    bool flag;  // ��Ĵ��״̬
    IMAGE img;  // ���ͼƬ
    IMAGE img2;//ͼƬ������ͼ
};
struct Ball3 {
    double x;  // ���x����
    double y;  // ���y����
    double r;  // ��İ뾶
    bool flag;  // ��Ĵ��״̬
    IMAGE img;  // ���ͼƬ
    IMAGE img2;//ͼƬ������ͼ
};

struct Ball food[food_num];//ʳ��ṹ��
struct Ball2 player[max_split];//��ҽṹ��  
struct Ball3 ai[Ai_num];//ai�ṹ��
struct Ball spitBalls[max_split][SPIT_BALL_NUM];//�³���ṹ��

// ��ͼƫ����
struct Offset {
    double x;//ˮƽƫ����
    double y;//��ֱƫ����
} offset;

POINT center;//���ĵ�

void GameInit() {//��Ϸ���ݳ�ʼ��
    srand((unsigned)time(NULL));//���������
    //��ʼ��ʳ��
    for (int i = 0; i < food_num; i++) {
        food[i].x = rand() % Width;
        food[i].y = rand() % Height;//�������λ��
        food[i].flag = true;
        food[i].r = 4;
        food[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);//�����ɫ
    }
    for (int i = 0; i < Ai_num; i++) {
        ai[i].x = rand() % Width;
        ai[i].y = rand() % Height;//�������λ��
        ai[i].flag = true;
        ai[i].r = 20;
        loadimage(&ai[i].img, _T("picture\\aiyin.jpg"), ai[i].r * 2, ai[i].r * 2, 1);
        loadimage(&ai[i].img2, _T("picture\\aiyin2.jpg"), ai[i].r * 2, ai[i].r * 2, 1);
    }
    player[0].x = rand() % Width;
    player[0].y = rand() % Height;  // ��������λ��
    player[0].flag = true;
    player[0].r = 20;
    player[0].color = RGB(rand() % 256, rand() % 256, rand() % 256);  // �����ɫ
    // ���������ĳ�ʼͼƬ
    loadimage(&player[0].img, _T("picture\\player.jpg"), player[0].r * 2, player[0].r * 2, 1);
    loadimage(&player[0].img2, _T("picture\\player2.jpg"), player[0].r * 2, player[0].r * 2, 1);
    for (int i = 1; i < max_split; i++) {
        player[i].flag = false;  // ��ʼ���������Ϊδ����״̬
    }

    // ��ʼ��ƫ������ʹ���λ����Ұ���ģ�
    offset.x = player[0].x - VIEW_WIDTH / 2;
    offset.y = player[0].y - VIEW_HEIGHT / 2;

    // ��ʼ���³���
    for (int j = 0; j < max_split; j++) {
        for (int i = 0; i < SPIT_BALL_NUM; i++) {
            spitBalls[j][i].flag = false;
        }
    }
}

void Gamedraw() {//����
    BeginBatchDraw();//�����������
    setbkcolor(BLACK);
    cleardevice();
    // ����ʳ��
    for (int i = 0; i < food_num; i++) {
        if (food[i].flag) {
            double drawX = food[i].x - offset.x;
            double drawY = food[i].y - offset.y;//ʳ����������-ƫ����=��ͼ����
            if (drawX >= 0 && drawX < VIEW_WIDTH && drawY >= 0 && drawY < VIEW_HEIGHT) {//�߽��飨����һ�����ػ��߽磩
                setfillcolor(food[i].color);
                solidcircle(drawX, drawY, food[i].r);
            }
        }
        else {//��ʳ�ﱻ�Ե�ʱ�����³�ʼ��ʳ��
            food[i].x = rand() % Width;
            food[i].y = rand() % Height;
            food[i].flag = true;
            food[i].r = 4;
            food[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
        }
    }
    // �������
    for (int i = 0; i < max_split; i++) {
        if (player[i].flag) {
            double playerDrawX = player[i].x - offset.x;
            double playerDrawY = player[i].y - offset.y;

            // ���¼��ز�����ͼƬ����Ӧ��Ĵ�С
            loadimage(&player[i].img, _T("picture\\player.jpg"), player[i].r * 2, player[i].r * 2, 1);
            loadimage(&player[i].img2, _T("picture\\player2.jpg"), player[i].r * 2, player[i].r * 2, 1);
            putimage(playerDrawX - player[i].r, playerDrawY - player[i].r, &player[i].img2, NOTSRCERASE);
            putimage(playerDrawX - player[i].r, playerDrawY - player[i].r, &player[i].img, SRCINVERT);

            settextcolor(WHITE);
            setbkmode(0);
            settextstyle(30, 0, L"΢���ź�");
            outtextxy(playerDrawX - 40, playerDrawY - player[i].r - 30, L"nagisaki");  // ��������Ϸ���ʾ�������
        }
    }
    // ����AI
    for (int i = 0; i < Ai_num; i++) {
        if (ai[i].flag) {
            double drawX = ai[i].x - offset.x;
            double drawY = ai[i].y - offset.y;
            if (drawX >= 0 && drawX < VIEW_WIDTH && drawY >= 0 && drawY < VIEW_HEIGHT) {//�߽��飨����һ�����ػ��߽磩
                loadimage(&ai[i].img, _T("picture\\aiyin.jpg"), ai[i].r * 2, ai[i].r * 2, 1);
                loadimage(&ai[i].img2, _T("picture\\aiyin2.jpg"), ai[i].r * 2, ai[i].r * 2, 1);
                putimage(drawX - ai[i].r, drawY - ai[i].r, &ai[i].img2, NOTSRCERASE);
                putimage(drawX - ai[i].r, drawY - ai[i].r, &ai[i].img, SRCINVERT);
            }
        }
    }
    //�����³���
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


//���ݲ���
struct mouse {//�������λ������ṹ��
    int x;
    int y;
}M;

void split(double x, double y, int i);//�������ѣ�������
void spit(int i);
int dir;//�������

// ���ƺ���
void Control() {
    ExMessage msg;//��Ϣ�ṹ��
    while (peekmessage(&msg)) {//������Ϣ
        if (msg.message == WM_MOUSEMOVE) {//����ƶ���Ϣ
            M.x = msg.x;
            M.y = msg.y;
        }
        else if (msg.message == WM_KEYDOWN) {//������Ϣ
            if (msg.vkcode == VK_SPACE) { // ���¿ո�����з���
                for (int i = 0; i < max_split; i++) {
                    if (player[i].flag) {
                        split(player[i].x, player[i].y, i);
                    }
                }
            }
            else if (msg.vkcode == VK_CONTROL) { // ����Ctrl����������
                for (int i = 0; i < max_split; i++) {
                    if (player[i].flag) {
                        spit(i);
                    }
                }
            }
        }
    }
}

struct next_position {//��һ��λ��
    double x;
    double y;
};
next_position N[max_split];//����max_split���򣬹�������

void changeDir(double x, double y) {//�ı䷽����
    int dX = M.x - (x - offset.x);
    int dY = M.y - (y - offset.y);//�����������Ұ���е�λ��
    dir = atan2(dY, dX);//����Ƕ�
}

const double SPEED = 1.9;//�����ٶ�
double getspeed(double speed, double r) {//�ı��ٶȣ��뾶Խ���ٶ�Խ��
    double current_speed = speed * (100 / (100 + r));
    return current_speed;//���ص�ǰ�ٶ�
}

bool canMove() {//�ж��Ƿ�����ƶ�  
    for (int i = 0; i < max_split; i++) {
        if (player[i].flag) {//����Ŀǰ���ڵ�������
            if (player[1].flag) {//�ж�����з��������
                center.x = (player[0].x + player[1].x) / 2;
                center.y = (player[0].y + player[1].y) / 2;//��������֮�����ĵ������             
            }
            if (!player[1].flag) {//���޷��������
                center.x = player[0].x;
                center.y = player[0].y;//�������ĵ�����Ϊĸ��Բ������
            }
            if (!player[0].flag) {//���޷��������
                center.x = player[1].x;
                center.y = player[1].y;//�������ĵ�����Ϊĸ��Բ������
            }
            changeDir(center.x, center.y);//ͨ�����ĵ����������㷽��Ƕ�
            N[i].x = player[i].x + (cos(dir) * getspeed(SPEED, player[i].r));
            N[i].y = player[i].y + (sin(dir) * getspeed(SPEED, player[i].r));//������õĽǶȼ���ÿ������һ����λ��
            //�߽紦��Ч���������߽練����
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
            //����һ����λ�ý����ж��ж�
            if (N[i].x < 0 + player[i].r || N[i].x > Width - player[i].r || N[i].y < 0 + player[i].r || N[i].y > Height - player[i].r) {
                return false;//��������߽��򷵻�false
            }
        }
    }
    return true;//�������е��ж��󷵻�true
}

void doMove() { // ����ƶ�   
    for (int i = 0; i < max_split; i++) {
        if (player[i].flag) {
            player[i].x = N[i].x;
            player[i].y = N[i].y;//ͨ������һ�������긳ֵ��playerʵ���ƶ�
        }
    }
    // ����ƫ������ʹ������ĵ�ʼ�մ�����Ұ���ģ�
    if (center.x - offset.x < VIEW_WIDTH / 2 - 1) {
        offset.x -= getspeed(SPEED, player[0].r);//������ҵ�ǰ���ٶȣ�ʹ��Ұ������ƶ�
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

    // �߽��飨�����Ұ����߽�͹̶�������
    if (offset.x < 0) offset.x = 0;
    if (offset.y < 0) offset.y = 0;
    if (offset.x > Width - VIEW_WIDTH) offset.x = Width - VIEW_WIDTH;
    if (offset.y > Height - VIEW_HEIGHT) offset.y = Height - VIEW_HEIGHT;
}

//����ƶ������ܷ�װ
void move() {
    if (canMove()) {
        doMove();
    }
}

// ���Ѻ���
void split(double x, double y, int i) {
    int active_balls = 0;
    for (int k = 0; k < max_split; k++) {
        if (player[k].flag) {
            active_balls++;//��¼��Ծ�������
        }
    }
    if (active_balls < max_split && player[i].r > 40) { // ���max_split�����Ұ뾶����40���ܷ���
        for (int j = 0; j < max_split; j++) {
            if (j != i && !player[j].flag) {
                player[j].flag = true;
                // ��ʼ��������ĳ�ʼλ�ã���ԭ������ͬ
                player[j].x = player[i].x + cos(dir) * (player[i].r + 50);
                player[j].y = player[i].y + sin(dir) * (player[i].r + 50);
                player[j].r = player[i].r / 2;
                player[j].color = player[i].color;
                player[i].r /= 2;//ԭ����İ뾶����
                break;
            }
        }
    }
}

// ������
void spit(int i) {
    if (player[i].r > 30) { // ��Ұ뾶����30��������
        for (int j = 0; j < SPIT_BALL_NUM; j++) {
            if (!spitBalls[i][j].flag) {
                // ��ʼ������ĳ�ʼλ�ã���ԭ������ͬ 
                spitBalls[i][j].x = player[i].x + cos(dir) * (player[i].r + 50);//�����³�������ԭ����ľ���
                spitBalls[i][j].y = player[i].y + sin(dir) * (player[i].r + 50);
                //�³���ı߽����
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
                player[i].r -= SPIT_BALL_RADIUS;  // ��С��Ұ뾶
                break;
            }
        }
    }
}

//��ʳ�ﲿ��

double getDistance(double x1, double x2, double y1, double y2) {//�������ĺ���
    double distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    return distance;
}


double V(double r) {//����������ĺ���
    double V = (4 * PI * pow(r, 3)) / 3;
    return V;
}

// ��ҳ���
// 1�����С�ڼ��ɳ�
// 2�����򸲸�С��ֱ����75%�ж�����
// 3������С������뾶����С��뾶��һ��
// 4������ʳ��뾶�̶�����0.5
// 5���������򣬰뾶�̶������³���İ뾶
void player_eat() {
    for (int j = 0; j < max_split; j++) {
        if (player[j].flag) {
            for (int i = 0; i < food_num; i++) { // ��ʳ��
                if (getDistance(food[i].x, player[j].x, food[i].y, player[j].y) <= player[j].r + food[i].r) {
                    food[i].flag = 0;
                    player[j].r += 0.5;
                }
            }
            for (int i = 0; i < Ai_num; i++) { // ��AI
                if (getDistance(ai[i].x, player[j].x, ai[i].y, player[j].y) - player[j].r <= ai[i].r / 4 && V(ai[i].r) <= V(player[j].r) && ai[i].flag == 1) {
                    ai[i].flag = 0;
                    player[j].r += ai[i].r / 2;
                }
            }

            // ��ҳ�����
            for (int k = 0; k < max_split; k++) {
                for (int i = 0; i < SPIT_BALL_NUM; i++) {
                    if (spitBalls[k][i].flag && getDistance(spitBalls[k][i].x, player[j].x, spitBalls[k][i].y, player[j].y) - player[j].r <= spitBalls[k][i].r / 4) {
                        spitBalls[k][i].flag = false;
                        player[j].r += spitBalls[k][i].r;
                    }
                }
            }

            // �����֮��Ļ��ԣ��ϲ����ƣ�
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

//ai����
const double ai_speed = 0.3;//����ai���ٶ�
int Dir;//ai�ƶ�����
//ai�ƶ�������ֻ��֪��Ŀ������꼴���ƶ���ԭ��ͬ����ƶ�
void aimove(double x, double y, int i) {
    if (ai[i].flag == 1) {
        double dx = x - ai[i].x;
        double dy = y - ai[i].y;
        Dir = atan2(dy, dx);
        double newX = ai[i].x + (cos(Dir) * getspeed(SPEED, ai[i].r));
        double newY = ai[i].y + (sin(Dir) * getspeed(SPEED, ai[i].r));
        // �߽���
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
        if (hit == 1) {//����������������
            newX = rand() % Width;
            newY = rand() % Height;
        }
        if (newX >= 0 + ai[i].r && newX < Width - ai[i].r && newY >= 0 + ai[i].r && newY < Height - ai[i].r) {//���б߽��ж�
            ai[i].x = newX;
            ai[i].y = newY;
        }
    }
}

//ai���ͬ������ҳ��
void ai_eat(int i) {
    if (ai[i].flag == 1) {
        for (int j = 0; j < food_num; j++) {//ai��ʳ��
            if (getDistance(food[j].x, ai[i].x, food[j].y, ai[i].y) <= ai[i].r + food[j].r) {
                food[j].flag = 0;
                food[j].color = BLACK;
                ai[i].r += 0.5;
            }
        }
        //ai��ai
        for (int k = 0; k < Ai_num; k++) {
            if (k != i && ai[k].flag == 1 && getDistance(ai[k].x, ai[i].x, ai[k].y, ai[i].y) - ai[i].r <= ai[k].r / 4 && V(ai[k].r) < V(ai[i].r)) {
                ai[k].flag = 0;
            }
        }
        //ai�����
        for (int j = 0; j < max_split; j++) {
            if (player[j].flag && getDistance(player[j].x, ai[i].x, player[j].y, ai[i].y) - ai[i].r <= player[j].r / 4 && V(player[j].r) < V(ai[i].r)) {
                player[j].flag = 0;
            }
        }
        //ai���³���
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

//Ѱ�������ʳ��
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

// Ѱ�����������AI
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

//Ѱ����Ҳ��Ե�
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

// Ѱ���������вAI
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

//�����ҵĹ���
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

void aiplay() { // ai��Ϊ�߼�,�ȶ�ܣ��ٹ���
    for (int i = 0; i < Ai_num; i++) {
        if (ai[i].flag == 1) {
            double target_x, target_y;//ai��Ŀ��λ��
            double threat_x=0, threat_y=0;
            double threat_x1, threat_y1;//ai�������в��AI��
            double threat_x2, threat_y2;//ai�������в����ң�
            // Ѱ���������в��AI��
            findNearestThreat(i, threat_x1, threat_y1);
            // Ѱ���������в����ң�
            findNearestThreat_player(i, threat_x2, threat_y2);

            // ѡ���������в
            if (getDistance(ai[i].x, threat_x1, ai[i].y, threat_y1) < getDistance(ai[i].x, threat_x2, ai[i].y, threat_y2)) {
                threat_x = threat_x1;
                threat_y = threat_y1;
            }
            if (getDistance(ai[i].x, threat_x1, ai[i].y, threat_y1) >= getDistance(ai[i].x, threat_x2, ai[i].y, threat_y2)) {
                threat_x = threat_x2;
                threat_y = threat_y2;
            }

            if (getDistance(ai[i].x, threat_x, ai[i].y, threat_y) < 50) {//����С��50��ʼ�Ӵ�
                double dx = ai[i].x - threat_x;
                double dy = ai[i].y - threat_y;//��������
                target_x = ai[i].x + dx;
                target_y = ai[i].y + dy;
            }
            else {//��ȷ����Χ��Σ�պ�
                bool foundPrey = false;
                // Ѱ����������AI��
                double prey_x1, prey_y1;
                findNearestPrey(i, prey_x1, prey_y1);
                // Ѱ������������ң�
                double prey_x2, prey_y2;
                findNearestPrey_player(i, prey_x2, prey_y2);

                // ѡ�����������
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
                    // û�����Ѱ�������ʳ��
                    findNearestFood(i, target_x, target_y);
                }
            }
            aimove(target_x, target_y, i);
        }
        ai_eat(i);
    }
}

//������淽��
void Start();//��ʾ��ʼ����
void showend();//��ʾ��������
void game_playing();//��Ϸ����
void showESC();//��ʾ��ͣ����
void victory();//��ʾʤ������

bool isPaused = 0;//������ͣ״̬

void game_playing() {//��Ϸ������
    bool gameover = 0;
    while (!gameover) {
        if (_kbhit()) {
            if (_getch() == 27) {//����ESC��ͣ
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
        bool die = 1;//�����������״̬
        for (int i = 0; i < max_split; i++) {
            if (player[i].flag) {
                die = 0;//ֻҪ��һ����Ҵ����ж�δ����
            }
        }
        if (die) {
            gameover = 1;
        }
        int count = 0; // ͳ�������� AI ����
        for (int i = 0; i < Ai_num; i++) {
            if (!ai[i].flag) {
                count++;
            }
        }
        if (count == Ai_num && !die) { // ���� AI ��������Ҵ��ж�ʤ��
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
    //�˳���Ϸ��
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

void showend() {//��ʾ��������
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
        // ������Ϸ״̬�����¿�ʼ
        GameInit();
        Start();
        };
    bool gameend = false;
    endButton.Onclick = [&gameend] {
        gameend = 1;
        closegraph();  // �ر�ͼ�δ���
        exit(0);       // �����˳�����
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

void victory() {//��ʾʤ������
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
        // ������Ϸ״̬�����¿�ʼ
        GameInit();
        Start();
        };
    bool gameend = false;
    endButton.Onclick = [&gameend] {
        gameend = 1;
        closegraph();  // �ر�ͼ�δ���
        exit(0);       // �����˳�����
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

//������
int main() {
    Start();
    GameInit();
    game_playing();
    showend();
}