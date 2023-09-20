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
	//��]���鎞�̃A���O��
	float m_angle;
	//���݂�HP
	int m_HP;
	//�~���������Ă�����c�莞��
	int m_timer;

	int m_stopTimer;

	float rotAngle = 0;
	static const int StopTime = 30;
	enum State
	{
		//�T�C�N�����ڎw��
		ToCiycle,
		//�T�C�N����𓮂�
		OnCiycle,
		//���S��ڎw���Ă���
		ToCenter,

		ReturnCiycle,
	};
	int m_state;//���݂̃X�e�[�^�X

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
		Shild,
		//����1
		Troop,
		Troop2,
		Boss,
		Bomb,
	};
	static const int BombLife = 600;
	int m_bombTimer = 0;
	bool isBombErase;

	float m_ext = 0.2f;

	int speedType;

	int m_returnNum = 0;
	bool m_isReturn = false;

	static int m_sprite[SpeedType::Bomb + 1];
	int nowSpriteNum = NormalMode;
	
	static int SpornAngle;

	bool isDelete = false;

	bool isStop = false;

	//���S����̒�~�p����
	static const int CenterR = 460;
	//�o������
	static const int OutsideR = 1000;
	//�ő�HP
	static const int MaxHP = 2;
	//�~���������Ă���Ƃ�
	static const int NormalMode = 6;
	//�U�����̐i�s���[�h
	static const int ProgressMode = 5;
	static const int FriendMode = 0;
	//���S�Ɍ������Ă����Ƃ��̑��x
	float m_ToCenterSpeed = 2;
	//�T�[�N���������Ă���Ƃ��̑��x
	static const int OnCiycleSpeed = 1;
	static const int MaxTimer = 120;
	static const int ShakeStartTime = 45;

	FLOAT2 shakePower;

	static FLOAT2 CiycleCenter;
	//�_���[�W�H�炤�]�[��
	static float TowerR;


	static void LoadFile();
	void Init(SpeedType type = SpeedType::Normal, int f_troopCount = 0);
	void Update();
	void Draw();
	void ToCiycleMove();
	void CiycleMove();
	void ReturnToCiycle();
	void BombUpdate();
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

	static const int NormalSpeedScore = 100;
	static const int MiddleSpeedScore = 125;
	static const int HiSpeedScore = 150;

	static bool isBoss;

	//static int enemyCiycle;
	static void Init();
	static void AddEnemy(BaseEnemy::SpeedType f_speedType);
	static void Update();
	static void Draw();
	static void CiycleInc();
	static void CiycleDec();
	static void AllDelete();
};
