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

	//void(*Onclick);����ָ��(c����)�����ڻص�������
	std::function<void()> Onclick;//������װ��(c++)

private:
	POINT m_position;//��ťλ��
	SIZE m_size;//��ť��С
	COLORREF m_color;//��ť��ɫ
};

