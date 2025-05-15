#pragma once
#include<easyx.h>
#include<functional>
class button
{
public:
    button(POINT position, SIZE size, COLORREF color);
    void Draw_start();
    void Draw_end();
    void Draw_return();
    void Event_Tick(const ExMessage* Ipmsg);

    std::function<void()> Onclick;

private:
    POINT m_position;
    SIZE m_size;
    COLORREF m_color;
    IMAGE img_start, img2_start;
    IMAGE img_end, img2_end;
    IMAGE img_return, img2_return;
};