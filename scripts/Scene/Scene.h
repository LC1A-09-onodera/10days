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
	static int m_s_encaenca;
	static int m_s_space;
	static int m_s_side;
	static FLOAT2 m_sideSize;
	static FLOAT2 m_titlePos;
	static FLOAT2 m_spacePos;
	static FLOAT2 m_titleSize;
	static FLOAT2 m_spaceSize;

	static float spaceAlpha;
	static bool isDec;

	static int m_s_ciycle;
	static FLOAT2 m_ciycleSize;
	static bool m_isEnca;
	static void LoadFile();
	static void Update();
	static void Init();
	static void Draw();
	static void SetRand();
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
	static int m_s_bossSide;
	//static FLOAT2 m_backPosition;
	//static FLOAT2 m_backSize;
	static FLOAT2 m_sideSize;
	static int sceneTimer;
	static const int dis = 50;
	static void LoadFile();
	static void Update();
	static void Init();
	static void Draw();
};

class ResultScene
{
public:
	static const int MaxCiycle = 350;
	static int ciycleR;
	static int s_space;
	static int s_score;
	static int s_number[10];
	static int m_score;
	static bool isToTitle;

	static float spaceAlpha;
	static bool isDec;
	
	static FLOAT2 m_numSize;
	static void LoadFile();
	static void Init(int f_score);
	static void Update();
	static void Draw();
	static void ResultToTitle();
};
