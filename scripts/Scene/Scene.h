#pragma once
#include "DxLib.h"

class TitleScene
{
	void Update();
};

class GameScene
{
public:
	enum class Direction
	{
		Right1,
		Right2,
		Left1,
		Left2,
	};
	static int m_s_backBround;
	static int m_s_side;
	static FLOAT2 m_backPosition;
	static FLOAT2 m_backSize;
	static FLOAT2 m_sidePosition[4];
	static FLOAT2 m_sideSize;
	static void LoadFile();
	static void Update();
	static void Init();
	static void Draw();
};
