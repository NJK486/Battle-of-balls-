#include "button.h"
#include <easyx.h>

button::button(POINT position, SIZE size, COLORREF color) {
	m_position = position;
	m_size = size;
	m_color = color;
}

void button::Draw_start() {
	IMAGE img;
	loadimage(&img, _T("picture\\startbutton.jpg"), m_size.cx , m_size.cy , 0);  // º”‘ÿÕº∆¨
	putimage(m_position.x, m_position.y, &img);  // œ‘ æÕº∆¨
}

void button::Draw_end() {
	IMAGE img;
	loadimage(&img, _T("picture\\endbutton.jpg"), m_size.cx , m_size.cy , 0);  // º”‘ÿÕº∆¨
	putimage(m_position.x, m_position.y, &img);  // œ‘ æÕº∆¨
}
void button::Draw_return() {
	IMAGE img;
	loadimage(&img, _T("picture\\returnbutton.jpg"), m_size.cx , m_size.cy , 0);  // º”‘ÿÕº∆¨
	putimage(m_position.x, m_position.y, &img);  // œ‘ æÕº∆¨
}

void button::Event_Tick(const ExMessage* Ipmsg) {
	if (WM_MOUSEMOVE <= Ipmsg->message && Ipmsg->message <= WM_MOUSEWHEEL) {
		m_color = WHITE;
		if (Ipmsg->x >= m_position.x && Ipmsg->x <= m_position.x + m_size.cx && Ipmsg->y >= m_position.y && Ipmsg->y <= m_position.y + m_size.cy) {
			if (Ipmsg->message == WM_MOUSEMOVE) {
				m_color = RED;
			}
			else if (Ipmsg->message == WM_LBUTTONDOWN) {
				m_color = BLUE;
				if (Onclick) {//ø’÷∏’Î
					Onclick();
				}
			}
		}
	}
}
