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

	int m_returnNum = 0;
	bool m_isReturn = false;

	static int m_sprite[7];
	int nowSpriteNum = NormalMode;
	
	static int SpornAngle;

	bool isDelete = false;

	//���S����̒�~�p����
	static const int CenterR = 450;
	//�o������
	static const int OutsideR = 1000;
	//�ő�HP
	static const int MaxHP = 5;
	//�~���������Ă���Ƃ�
	static const int NormalMode = 6;
	//�U�����̐i�s���[�h
	static const int ProgressMode = 5;
	static const int FriendMode = 0;
	//���S�Ɍ������Ă����Ƃ��̑��x
	static const int ToCenterSpeed = 2;
	//�T�[�N���������Ă���Ƃ��̑��x
	static const int OnCiycleSpeed = 1;
	static const int MaxTimer = 600;
	static FLOAT2 CiycleCenter;
	//�_���[�W�H�炤�]�[��
	static float TowerR;

	static void LoadFile();
	void Init();
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
	static std::list<BaseEnemy *> enemys;
	static std::list<std::list<BaseEnemy*>::iterator> deleteEnemys;
	static void Init();
	static void AddEnemy();
	static void Update();
	static void Draw();
};
