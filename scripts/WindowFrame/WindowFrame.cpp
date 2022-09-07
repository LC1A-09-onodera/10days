#include "WindowFrame.h"

int WindowFrame::m_sprite[2];
FLOAT2 WindowFrame::m_nowPosition[2];
FLOAT2 WindowFrame::m_goalPosition[2];
FLOAT2 WindowFrame::m_startPosition[2];

void WindowFrame::LoadFile()
{
	m_sprite[static_cast<int>(Direction::RIGHT)] = LoadGraph("Resources/.png");
	m_sprite[static_cast<int>(Direction::LEFT)] = LoadGraph("Resources/.png");
}

void WindowFrame::Update(int f_nowScore)
{
	
}

void WindowFrame::Draw()
{
	DrawExtendGraph( 0, 0, 10, 10, m_sprite[static_cast<int>(Direction::RIGHT)], true);
	DrawExtendGraph(0, 0, 10, 10, m_sprite[static_cast<int>(Direction::LEFT)], true);
}
