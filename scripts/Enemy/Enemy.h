#pragma once
#include "DxLib.h"
#include <list>

class BaseEnemy
{
protected:
	//
	FLOAT2 m_position;
	FLOAT2 m_endPosition;
	float m_easeTimer;
	//
	FLOAT2 m_size;
	//回転する時のアングル
	float m_angle;
	//中心からの停止用距離
	float centerR;
	//
	FLOAT2 m_speed;

	int m_timer;

	void Init(FLOAT2 f_position ,FLOAT2 f_speed = {1.0f, 1.0f}, int f_timer = 120, float f_centerR = 350.0f);
	void Update();
	void Draw(int graph);
	void CiycleMove();
	void LineMove();
};

class Enemy1 : public BaseEnemy
{
	static int sprite;
};

class EnemyManager
{
	static std::list<BaseEnemy *> enemys;
	static void Update();
	static void Draw();
};