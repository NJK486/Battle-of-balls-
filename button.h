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

	//void(*Onclick);函数指针(c语言)（用于回调函数）
	std::function<void()> Onclick;//函数包装器(c++)

private:
	POINT m_position;//按钮位置
	SIZE m_size;//按钮大小
	COLORREF m_color;//按钮颜色
};

