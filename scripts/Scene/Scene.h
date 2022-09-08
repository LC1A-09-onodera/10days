#pragma once
#include "DxLib.h"

class SceneSide
{
public:
	static FLOAT2 m_sidePosition[4];
};

class TitleScene
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
	static int m_s_title;
	static int m_s_space;
	static int m_s_side;
	static FLOAT2 m_sideSize;
	static FLOAT2 m_titlePos;
	static FLOAT2 m_spacePos;
	static FLOAT2 m_titleSize;
	static FLOAT2 m_spaceSize;

	static int m_s_ciycle;
	static FLOAT2 m_ciycleSize;
	static void LoadFile();
	static void Update();
	static void Init();
	static void Draw();
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
	//static int m_s_backBround;
	static int m_s_side;
	//static FLOAT2 m_backPosition;
	//static FLOAT2 m_backSize;
	static FLOAT2 m_sideSize;
	static void LoadFile();
	static void Update();
	static void Init();
	static void Draw();
};
