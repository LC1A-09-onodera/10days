#pragma once
#include "DxLib.h"

class WaveManager
{
private:
public:
	static const int MaxWaveNum = 10;
	static int waveNumber;
	static int s_waves[MaxWaveNum];

	static int s_back;
	static FLOAT2 m_backSize;
	static float m_backEaseTimer;
	static FLOAT2 m_backEndSize;
	static bool isAllEnd;
	static bool isBackEnd;
	
	//演出開始
	static bool isWaveDirectionStart;
	//演出終了
	static bool isWaveDirectionEnd;
	//次のウェーブにいく予兆
	static bool isToNextWave;
	static float easeTimer;

	static const int stopTime = 30;
	static int stopTimer;
	static bool isStopEnd;

	static int WaveBorader[MaxWaveNum - 1];

	static int s_rule;
	static int ruleAlpha;
	static float ruleEaseTimer;
	static bool isRuleEnd;
	static FLOAT2 nowRuleSize;
	static FLOAT2 MaxRuleSize;

	static int s_change;
	static int changeAlpha;
	static float changeEaseTimer;
	static bool isChangeEnd;
	static FLOAT2 nowChangeSize;
	static FLOAT2 MaxChangeSize;

	static int s_transe;
	static int transeAlpha;
	static float transeEaseTimer;
	static bool isTranseEnd;
	static FLOAT2 nowTranseSize;
	static FLOAT2 MaxTranseSize;

	static int s_bomb;
	static int bombAlpha;
	static float bombEaseTimer;
	static bool isBombEnd;
	static FLOAT2 nowBombSize;
	static FLOAT2 MaxBombSize;


	static FLOAT2 m_size;
	static FLOAT2 m_position;
	static FLOAT2 m_endPosition;

	static bool isBombHit;
	static const int BombHitTime = 10;
	static int bombTimer;

	static void LoadFile();

	static void WaveInit(int waveNum);
	static void Update();
	static void Draw();

	static void Wave1();
	static void Wave2();
	static void Wave3();
	static void Wave4();
	static void Wave5();
	static void Wave6();
	static void Wave7();
	static void Wave8();
	static void Wave9();
	static void Wave10();
private:
	
};

