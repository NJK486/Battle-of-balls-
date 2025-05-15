#pragma once
#include<easyx.h>
class scene
{
public:
    scene(POINT p_position, SIZE p_size);
    void draw_start();
    void show_end();
    void showESC();
    void victory();
private:
    POINT p_position;
    SIZE p_size;
};