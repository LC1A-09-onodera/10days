#pragma once
#include "DxLib.h"
#include <list>
#include "../WindowsSize/WindowSize.h"
class BaseEnemy
{
public:
	//
	FLOAT2 m_position;
	FLOAT2 m_endPosition;
	float m_easeTimer;
	//
	FLOAT2 m_size;
	//回転する時のアングル
	float m_angle;
	//現在のHP
	int m_HP;
	//円周上を回っていられる残り時間
	int m_timer;

	int m_stopTimer;
	static const int StopTime = 30;
	enum State
	{
		//サイクル上を目指す
		ToCiycle,
		//サイクル上を動く
		OnCiycle,
		//中心を目指していく
		ToCenter,

		ReturnCiycle,
	};
	int m_state;//現在のステータス

	enum Type
	{
		Devil,
		Angel
	};
	int m_type = Devil;

	enum SpeedType
	{
		Normal,
		Midl,
		Hi,
	};

	int speedType;

	int m_returnNum = 0;
	bool m_isReturn = false;

	static int m_sprite[3];
	int nowSpriteNum = NormalMode;
	
	static int SpornAngle;

	bool isDelete = false;

	bool isStop = false;

	//中心からの停止用距離
	static const int CenterR = 470;
	//出現時の
	static const int OutsideR = 1000;
	//最大HP
	static const int MaxHP = 1;
	//円周上を回っているとき
	static const int NormalMode = 6;
	//攻撃時の進行モード
	static const int ProgressMode = 5;
	static const int FriendMode = 0;
	//中心に向かっていくときの速度
	float m_ToCenterSpeed = 2;
	//サークル場を回っているときの速度
	static const int OnCiycleSpeed = 1;
	static const int MaxTimer = 120;
	static const int ShakeStartTime = 45;

	FLOAT2 shakePower;

	static FLOAT2 CiycleCenter;
	//ダメージ食らうゾーン
	static float TowerR;


	static void LoadFile();
	void Init(SpeedType type = SpeedType::Normal);
	void Update();
	void Draw();
	void ToCiycleMove();
	void CiycleMove();
	void ReturnToCiycle();
	void LineMove();
	void HitShiled();
	void BulletCollision();
};


class EnemyManager
{
public:
	EnemyManager();
	static std::list<BaseEnemy *> enemys;
	static std::list<std::list<BaseEnemy*>::iterator> deleteEnemys;

	static int nowTowerR;
	static int nowCenterR;

	static const int MaxR = 1500;

	//static int enemyCiycle;
	static void Init();
	static void AddEnemy();
	static void Update();
	static void Draw();
	static void CiycleInc();
	static void CiycleDec();
	static void AllDelete();
};
