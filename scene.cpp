#include "scene.h"

scene::scene(POINT position, SIZE size) {
    p_position = position;
    p_size = size;
}
void scene::draw_start() {
    IMAGE img;
    loadimage(&img, _T("picture\\Start.jpg"),p_size.cx+p_position.x,p_size.cy+p_position.y,0);  // º”‘ÿÕº∆¨
    putimage(p_position.x, p_position.y,&img);  // œ‘ æÕº∆¨
}

void scene::show_end() {
    IMAGE img;
    loadimage(&img, _T("picture\\game_over.jpg"), p_size.cx + p_position.x, p_size.cy + p_position.y, 0);
    putimage(p_position.x,p_position.y,&img);
}

void scene::showESC() {
    IMAGE img;
    loadimage(&img, _T("picture\\nagisaki.jpg"), p_size.cx + p_position.x, p_size.cy + p_position.y, 0);  // º”‘ÿÕº∆¨
    putimage(p_position.x, p_position.y, &img);  // œ‘ æÕº∆¨
}