#pragma once
#include "DxLib.h"
#include "../Particle/Particle.h"
#include "../WindowsSize/WindowSize.h"

class Player
{
private:
	//�X�v���C�g
	int m_sprite;
	FLOAT2 m_position = { 0.0f, 0.0f };

	FLOAT2 m_size = { 256.0f, 256.0f };
	//�G�̌���
	FLOAT2 m_rotation = { 0.0f, 0.0f };
	//���Z�������ɉ��������
	FLOAT2 m_addForce = { 0.0f, 0.0f };
	//���Z�ppower
	FLOAT2 m_subForce = { 0.0f, 0.0f };
	//�W�F�b�g�p�b�N�̌���
	FLOAT2 m_jettRotation = { 0.0f, 0.0f };
	//���Z�d��
	FLOAT2 m_gravity = { 0.0f, 0.0f };
	//����
	FLOAT2 m_direction = { 0.0f, 0.0f };

	const FLOAT2 m_winSize = { 1280, 920 };
	//�X�N���[���J�n�ʒu
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
	const int C_STAGE_RAD = 243;					//�X�e�[�W�̔��a
	const int C_BULLET_INIT_VAL = 74;				//�e�����l
	const float C_PLAYER_RAD = 20.0f;				//�v���C���[�̔��a
	const float C_TOTAL_RAD =
		C_STAGE_RAD + C_PLAYER_RAD;		//���g�p
	const float C_HALF_WID = WindowSize::Wid / 2;	//�E�B���h�E�T�C�Y������
	const float C_HALF_HEI = WindowSize::Hi / 2;	//�E�B���h�E�T�C�Y�c����
	const float C_ADD_TIMER = 0.02f;	//�C�[�W���O���Z�^�C�}�[
	const float C_SUB_RAD = 2.0f;		//�����@�̔��a�̌��Z�p
	const float C_LINE_WID = 7.0f;		//�X�e�[�W�̐��̕�

private:
	FLOAT2 m_start_pos;
	FLOAT2 m_end_pos;
	FLOAT2 m_vec;
	int m_stage_rad;
	int m_bulletNum;
	int m_maxBulletNum;
	float m_easeTimer;
	float m_deg;
	bool m_isMove;
	bool m_isReload;

	int m_s_player;
	int m_s_stage;
	FLOAT2 m_stageSize;

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
	const float GetDeg() { return m_deg; }
	const int& GetBulletNum() { return m_bulletNum; }
	const int& GetMaxBulletNum() { return m_maxBulletNum; }

public:
	const void SetStageSize(const FLOAT2& stageSize) { m_stageSize = stageSize; }
	const void SetStageRad(const int& stageRad) { m_stage_rad = stageRad; }
	const void AddPlayerPosY(const float& addPosY) { m_position.v += addPosY; }
};
