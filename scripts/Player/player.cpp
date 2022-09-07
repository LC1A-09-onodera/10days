#include "player.h"
#include "../Input/Input.h"
#include "../Scroll/Scroll.h"

void Player::Init()
{
	m_position = {
		C_HALF_WID,
		C_HALF_HEI + C_STAGE_RAD - C_PLAYER_RAD };
	m_gravity.v = 0.9f;
	m_scrollStartLine = { m_winSize.u / 2, 0 };

	m_spaceCount = 2;
	m_easeTimer = 0.0f;
	m_side = OUTSIDE;
	m_loc = LOWER;
	m_isMove = false;
}

void Player::Update()
{
	AddForce();

	AttachForce();

	if (!m_isMove)
	{
		if (Input::GetKeyTrigger(KEY_INPUT_SPACE))
		{
			//�c�ړ�
			if (m_spaceCount >= 2)
			{
				m_spaceCount = 0;
				m_isMove = true;
			}
			//���O�ړ�
			else
			{
				if (m_side == OUTSIDE)
				{
					//���œ�����O�̏ꍇ
					if (m_loc == LOWER) { m_position.v += C_PLAYER_RAD * 2; }
					//��œ�����O�̏ꍇ
					else { m_position.v -= C_PLAYER_RAD * 2; }
					m_side = INSIDE;
				}
				else
				{
					//���ŊO������̏ꍇ
					if (m_loc == LOWER) { m_position.v -= C_PLAYER_RAD * 2; }
					//��ŊO������̏ꍇ
					else { m_position.v += C_PLAYER_RAD * 2; }
					m_side = OUTSIDE;
				}
				m_spaceCount++;
			}
		}
	}

	//�c�ړ����̏���
	else
	{
		//���Z����
		if (m_easeTimer < 1.0f) { m_easeTimer += C_ADD_TIMER; }

		//�������
		if (m_loc == LOWER)
		{
			const float l_start = C_HALF_HEI + C_STAGE_RAD - C_PLAYER_RAD;
			const float l_end = C_HALF_HEI - C_STAGE_RAD + C_PLAYER_RAD;
			m_position.v = (l_end - l_start) * easeInOutSine(m_easeTimer) + l_start;
			if (m_easeTimer >= 1.0f)
			{
				m_easeTimer = 0.0f;
				m_isMove = false;
				m_loc = UPPER;
			}
		}
		//�ォ�牺
		else
		{
			const float l_start = C_HALF_HEI - C_STAGE_RAD + C_PLAYER_RAD;
			const float l_end = C_HALF_HEI + C_STAGE_RAD - C_PLAYER_RAD;
			m_position.v = (l_end - l_start) * easeInOutSine(m_easeTimer) + l_start;
			if (m_easeTimer >= 1.0f)
			{
				m_easeTimer = 0.0f;
				m_isMove = false;
				m_loc = LOWER;
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

	//�����@
	DrawCircleAA(m_position.u, m_position.v, C_PLAYER_RAD, 100, GetColor(255, 255, 0), true);

	//���X�e�[�W
	DrawCircle(640, 360, C_STAGE_RAD, GetColor(255, 255, 255), false);
}

void Player::LoadFile()
{
	m_sprite = LoadGraph("Resources/particle.png");
	Init();
}

bool Player::GetIsMove()
{
	return m_isMove;
}

bool Player::GetIsSide()
{
	return m_side;
}

void Player::AddForce()
{
	
}

void Player::AttachForce()
{

}
