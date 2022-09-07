#pragma once
#include "DxLib.h"
#include "../Particle/Particle.h"

class Player
{
private:
	//スプライト
	int m_sprite;
	FLOAT2 m_position = { 0.0f, 0.0f };

	FLOAT2 m_size = { 256.0f, 256.0f };
	//絵の向き
	FLOAT2 m_rotation = { 0.0f, 0.0f };
	//加算処理時に加えられる力
	FLOAT2 m_addForce = { 0.0f, 0.0f };
	//減算用power
	FLOAT2 m_subForce = { 0.0f, 0.0f };
	//ジェットパックの向き
	FLOAT2 m_jettRotation = { 0.0f, 0.0f };
	//加算重力
	FLOAT2 m_gravity = { 0.0f, 0.0f };
	//向き
	FLOAT2 m_direction = { 0.0f, 0.0f };

	const FLOAT2 m_winSize = { 1280, 720 };
	//スクロール開始位置
	FLOAT2 m_scrollStartLine = { m_winSize.u / 2, 0 };

private:
	enum SIDE
	{
		OUTSIDE, INSIDE
	};
	enum LOC
	{
		LOWER, UPPER
	};

private:
	const int C_STAGE_RAD = 300;
	const float C_PLAYER_RAD = 20.0f;
	const float C_TOTAL_RAD = C_STAGE_RAD + C_PLAYER_RAD;

	const float C_HALF_WID = 640.0f;
	const float C_HALF_HEI = 360.0f;

	const float C_ADD_TIMER = 0.02f;

private:
	int m_spaceCount;
	float m_easeTimer;
	bool m_side;
	bool m_loc;
	bool m_isMove;

public:
	void Init();
	void Update();
	void Draw();
	void LoadFile();
	bool GetIsMove();

private:
	void AddForce();
	void AttachForce();

private:
	float easeInOutSine(float x)
	{
		return -(cosf(3.14f * x) - 1) / 2;
	}

public:
	FLOAT2 GetPos() { return m_position; }
};
