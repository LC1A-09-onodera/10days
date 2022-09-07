#pragma once

#include "DxLib.h"

class WindowFrame
{
	enum class Direction
	{
		RIGHT,
		LEFT,
	};
	static int m_sprite[2];
	static FLOAT2 m_nowPosition[2];
	static FLOAT2 m_goalPosition[2];
	static FLOAT2 m_startPosition[2];
	static float easeTimer;
	static void LoadFile();
	static void Update(int f_nowScore);
	static void Draw();
};