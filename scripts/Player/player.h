#pragma once
#include "DxLib.h"
#include "../Particle/Particle.h"
#include "../WindowsSize/WindowSize.h"

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

	const FLOAT2 m_winSize = { 1280, 920 };
	//スクロール開始位置
	FLOAT2 m_scrollStartLine = { m_winSize.u / 2, 0 };

private:
	const int C_STAGE_RAD = 243;					//ステージの半径
	const int C_STAGE_REFLECTOR_RAD = 300;			//中心から見たリフレクターの位置の半径
	const int C_REFLECTOR_RAD = 15;					//リフレクターの半径(テクスチャ依存)
	const int C_BULLET_INIT_VAL = 72;				//弾初期値
	const float C_PLAYER_RAD = 20.0f;				//プレイヤーの半径
	const float C_TOTAL_RAD =
		C_STAGE_RAD + C_PLAYER_RAD;		//未使用
	const float C_HALF_WID = WindowSize::Wid / 2;	//ウィンドウサイズ横半分
	const float C_HALF_HEI = WindowSize::Hi / 2;	//ウィンドウサイズ縦半分
	const float C_ADD_TIMER = 0.02f;	//イージング加算タイマー
	const float C_SUB_RAD = 2.0f;		//仮自機の半径の減算用
	const float C_LINE_WID = 7.0f;		//ステージの線の幅

private:
	FLOAT2 m_start_pos;
	FLOAT2 m_end_pos;
	FLOAT2 m_vec;
	FLOAT2 m_reflector_pos;
	int m_stage_rad;
	int m_bulletNum;
	int m_maxBulletNum;
	float m_easeTimer;
	float m_deg;
	float m_reflector_rad;
	bool m_isMove;
	bool m_isReload;

	int m_s_player;
	int m_s_stage;
	int m_s_reflector;
	FLOAT2 m_stageSize;

	//debug
	float a = 0;

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
	float RotateEarliestArc(float NowAngle, float EndAngle)
	{
		if (fabsf(EndAngle - NowAngle) > 180.0f) {
			if (NowAngle < 180.0f) {
				NowAngle += 360.0f;
			}
			else {
				NowAngle -= 360.0f;
			}
		}
		return EndAngle - NowAngle;
	}

public:
	bool ShotBullet();
	const FLOAT2& GetPos() { return m_position; }
	const FLOAT2& GetStageSize() { return m_stageSize; }
	const FLOAT2& GetReflectorPos() { return m_reflector_pos; }
	const FLOAT2& GetHalfWinSize() { return FLOAT2{ C_HALF_WID,C_HALF_HEI }; }
	const float& GetReflectorRad() { return m_reflector_rad; }
	const float& GetDeg() { return m_deg; }
	const int& GetBulletNum() { return m_bulletNum; }
	const int& GetMaxBulletNum() { return m_maxBulletNum; }
	const int& GetStageReflectorRad() { return C_STAGE_REFLECTOR_RAD; }

public:
	const void SetStageSize(const FLOAT2& stageSize) { m_stageSize = stageSize; }
	const void SetStageRad(const int& stageRad) { m_stage_rad = stageRad; }
	const void AddPlayerPosY(const float& addPosY) { m_position.v += addPosY; }
};
