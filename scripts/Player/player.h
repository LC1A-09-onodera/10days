#pragma once
#include "DxLib.h"
#include "../Particle/Particle.h"

class Player
{
private:
	//�X�v���C�g
	int m_sprite;
	FLOAT2 m_position = { 0.0f, 0.0f };

	FLOAT2 m_size = {256.0f, 256.0f};
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

	const FLOAT2 m_winSize = { 1280, 720 };
	//�X�N���[���J�n�ʒu
	FLOAT2 m_scrollStartLine = {m_winSize.u / 2, 0};
public:
	void Init();
	void Update();
	void Draw();
	void LoadFile();

private:
	void AddForce();
	void AttachForce();
	
};
