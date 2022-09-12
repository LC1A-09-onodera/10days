#include "player.h"
#include "../Input/Input.h"
#include "../Scroll/Scroll.h"

void Player::Init()
{
	//�ʒu,��
	m_position = {
		C_HALF_WID,
		C_HALF_HEI + C_STAGE_RAD - C_PLAYER_RAD };
	m_gravity.v = 0.9f;
	m_scrollStartLine = { m_winSize.u / 2, 0 };

	m_start_pos = { 0,0 };
	m_vec = { 0,0 };
	m_end_pos = { 0,0 };
	m_reflector_pos = {
		C_HALF_WID,
		C_HALF_HEI + C_STAGE_REFLECTOR_RAD };
	m_stage_rad = C_STAGE_RAD;
	m_bulletNum = C_BULLET_INIT_VAL;
	m_maxBulletNum = m_bulletNum;
	m_stage_rad = C_STAGE_RAD;
	m_rad = 1.0f;
	m_easeTimer = 0.0f;
	m_deg = 0;
	m_reflector_rad = DX_PI_F;
	m_isMove = false;
	m_stageSize = { 504, 504 };
	m_isReload = false;
	m_isChange = false;
	m_isReflectorHit = false;
}

void Player::Update()
{
	AddForce();

	AttachForce();

	if (m_bulletNum == 0)
	{
		m_bulletNum = m_maxBulletNum;
	}

	//�ʏ펞
	if (!m_isMove)
	{
		//���@�̕����x�N�g�����v�Z
		FLOAT2 l_diff = { 0,0 };
		l_diff.u = m_position.u - C_HALF_WID;
		l_diff.v = m_position.v - C_HALF_HEI;
		float l_len = sqrtf(
			powf(l_diff.u, 2.0f) +
			powf(l_diff.v, 2.0f));
		FLOAT2 l_vec = { 0,0 };
		l_vec.u = l_diff.u / l_len;
		l_vec.v = l_diff.v / l_len;
		float l_pRad = atan2f(-l_vec.v, -l_vec.u);
		if (l_pRad < 0.0f)
		{
			l_pRad += DX_PI_F * 2;
		}
		m_deg = 180.0f / DX_PI_F * l_pRad;

		//���X�e�B�b�N���|����Ă��鎞�̂�(�R���g���[���[�ȊO���Ή�������I)
		if (Input::isJoyLeftStickBottom())
		{
			//���@�̈ʒu�Z�o���K��
			m_vec = Input::GetJoyLeftStick();
			float l_len = sqrtf(powf(m_vec.u, 2.0f) + powf(m_vec.v, 2.0f));
			m_vec.u /= l_len;
			m_vec.v /= l_len * -1;

			float l_pAngle = 180.0f / DX_PI_F * atan2f(l_vec.v, l_vec.u);
			float l_sAngle = 180.0f / DX_PI_F * atan2f(m_vec.v, m_vec.u);
			if (l_pAngle < 0.0f) { l_pAngle += 360.0f; }
			if (l_sAngle < 0.0f) { l_sAngle += 360.0f; }
			float l_nearArc = RotateEarliestArc(l_pAngle, l_sAngle);

			FLOAT2 l_nearVec = { 0,0 };
			//�v�C��
			float l_rad = (l_pAngle + (l_nearArc / 30.0f)) * DX_PI_F / 180.0f;
			l_nearVec.u = cosf(l_rad);
			l_nearVec.v = sinf(l_rad);

			m_position.u = l_nearVec.u * (C_STAGE_RAD - C_PLAYER_RAD) + C_HALF_WID;
			m_position.v = l_nearVec.v * (C_STAGE_RAD - C_PLAYER_RAD) + C_HALF_HEI;

			//���t���N�^�[
			m_reflector_pos.u = l_nearVec.u * C_STAGE_REFLECTOR_RAD + C_HALF_WID;
			m_reflector_pos.v = l_nearVec.v * C_STAGE_REFLECTOR_RAD + C_HALF_HEI;
			m_reflector_rad = l_pRad - DX_PI_F / 2.0f;
			if (m_reflector_rad < 0.0f)
			{
				m_reflector_rad += DX_PI_F * 2.0f;
			}
		}

		//�c�f����
		if (Input::GetKeyTrigger(KEY_INPUT_SPACE) ||
			Input::isJoyBottomTrigger(XINPUT_BUTTON_A))
		{
			m_bulletNum = m_maxBulletNum;
			m_start_pos = m_position;
			l_vec.u *= -1.0f;
			l_vec.v *= -1.0f;
			m_end_pos.u = l_vec.u * (C_STAGE_RAD - C_PLAYER_RAD) + C_HALF_WID;
			m_end_pos.v = l_vec.v * (C_STAGE_RAD - C_PLAYER_RAD) + C_HALF_HEI;
			m_vec = l_vec;
			m_isMove = true;
		}

		//���t���N�^�[�q�b�g��
		if (m_isReflectorHit)
		{
			FLOAT2 l_shakePower = { 1.0f,1.0f };
			Shake::AddShakePower(l_shakePower);
			m_isReflectorHit = false;
		}
	}

	//�c�f�ړ���
	else
	{
		//�^�C�}�[���Z
		if (m_easeTimer < 1.0f) { m_easeTimer += C_ADD_TIMER; }

		//�ړ�����
		m_position.u = (m_end_pos.u - m_start_pos.u) * easeInOutSine(m_easeTimer) + m_start_pos.u;
		m_position.v = (m_end_pos.v - m_start_pos.v) * easeInOutSine(m_easeTimer) + m_start_pos.v;
		if (m_easeTimer >= 1.0f)
		{
			m_easeTimer = 0.0f;
			m_isMove = false;
		}

		//���@�g�k����
		if (!m_isChange)
		{
			if (m_rad > 0.0f)
			{
				m_rad -= C_ADD_TIMER * 4.0f;
			}
			if (m_rad < 0.0f) { m_rad = 0.0f; }
			if (m_easeTimer >= 0.5f)
			{
				m_reflector_pos.u = m_vec.u * C_STAGE_REFLECTOR_RAD + C_HALF_WID;
				m_reflector_pos.v = m_vec.v * C_STAGE_REFLECTOR_RAD + C_HALF_HEI;
				m_reflector_rad -= DX_PI_F;
				m_isChange = true;
			}
		}
		else
		{
			if (m_rad < 1.0f)
			{
				m_rad += C_ADD_TIMER * 4.0f;
			}
			if (m_rad > 1.0f) { m_rad = 1.0f; }
			if (!m_isMove)
			{
				m_isChange = false;
			}
		}
	}
}

void Player::Draw()
{
	//DrawGraph(static_cast<int>(m_position.u) - Scroll::GetScrollX(), static_cast<int>(m_position.v), m_sprite, true);

	float left = Input::GetJoyLeftTrigger();
	float right = Input::GetJoyRightTrigger();
	DrawFormatString(0, 0, GetColor(255, 255, 255), "LEFT:%2f", left);
	DrawFormatString(0, 20, GetColor(255, 255, 255), "RIGHT:%2f", right);

	//���@
	DrawRotaGraph(
		m_position.u + Shake::GetShake().u,
		m_position.v + Shake::GetShake().v,
		m_rad * 0.3f,
		m_reflector_rad - DX_PI_F,
		m_s_player,
		true
	);

	//���t���N�^�[
	DrawRotaGraph(
		m_reflector_pos.u + Shake::GetShake().u,
		m_reflector_pos.v + Shake::GetShake().v,
		m_rad,
		m_reflector_rad,
		m_s_reflector,
		true
	);

	//�X�e�[�W
	DrawExtendGraph(
		(WindowSize::Wid / 2) + Shake::GetShake().u - m_stageSize.u / 2.0f,
		(WindowSize::Hi / 2) + Shake::GetShake().v - m_stageSize.v / 2.0f,
		(WindowSize::Wid / 2) + Shake::GetShake().u + m_stageSize.u / 2.0f,
		(WindowSize::Hi / 2) + Shake::GetShake().v + m_stageSize.v / 2.0f,
		m_s_stage,
		true
	);

	//debug
	float hoge = Shake::GetPowerX();
	DrawFormatString(0, 40, GetColor(0, 0, 0), "ShakeX:%f", hoge);
	DrawFormatString(0, 60, GetColor(0, 0, 0), "RefRad:%f", m_rad);
}

void Player::LoadFile()
{
	m_s_player = LoadGraph("Resources/player.png");
	m_sprite = LoadGraph("Resources/particle.png");
	m_s_stage = LoadGraph("Resources/circle.png");
	m_s_reflector = LoadGraph("Resources/reflector.png");
	Init();
}

bool Player::GetIsMove()
{
	return m_isMove;
}

void Player::AddForce()
{

}

void Player::AttachForce()
{

}

void Player::ReflectorHit()
{
	m_isReflectorHit = true;
}

bool Player::ShotBullet()
{
	if (m_bulletNum > 0)
	{
		m_bulletNum--;
		return true;
	}
	return false;
}
