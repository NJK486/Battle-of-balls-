#include "scene.h"

scene::scene(POINT position, SIZE size) {
    p_position = position;
    p_size = size;
}
void scene::draw_start() {
    IMAGE img;
    loadimage(&img, _T("picture\\Start.jpg"), p_size.cx + p_position.x, p_size.cy + p_position.y, 0);  // ����ͼƬ
    putimage(p_position.x, p_position.y, &img);  // ��ʾͼƬ
}

void scene::show_end() {
    IMAGE img;
    loadimage(&img, _T("picture\\end.jpg"), p_size.cx + p_position.x, p_size.cy + p_position.y, 0);
    putimage(p_position.x, p_position.y, &img);
}

void scene::showESC() {
    IMAGE img;
    IMAGE img2;
    loadimage(&img, _T("picture\\nagisaki.jpg"), p_size.cx + p_position.x, p_size.cy + p_position.y, 0);  // ����ͼƬ
    loadimage(&img2, _T("picture\\nagisaki2.jpg"), p_size.cx + p_position.x, p_size.cy + p_position.y, 0);
    putimage(p_position.x, p_position.y, &img2, NOTSRCERASE);  // ��ʾͼƬ
    putimage(p_position.x, p_position.y, &img, SRCINVERT);  // ��ʾͼƬ
}

void scene::victory() {
    IMAGE img;
    loadimage(&img, _T("picture\\victory.jpg"), p_size.cx + p_position.x, p_size.cy + p_position.y, 0);  // ����ͼƬ
    putimage(p_position.x, p_position.y, &img);  // ��ʾͼƬ
}