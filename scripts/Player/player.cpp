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

	m_outside_pos = { C_HALF_WID,0 };
	m_spaceCount = 2;
	m_easeTimer = 0.0f;
	m_outside_rad = 0.0f;
	m_side = OUTSIDE;
	m_loc = LOWER;
	m_isMove = false;
	m_stageSize = {504, 504};
	m_isChange = false;
}

void Player::Update()
{
	AddForce();

	AttachForce();

	//���O�ړ��̏���
	if (!m_isMove)
	{
		if (Input::GetKeyTrigger(KEY_INPUT_SPACE) || Input::isJoyBottomTrigger(XINPUT_BUTTON_A))
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
				//���O�ړ����o�p
				m_isChange = true;

				if (m_side == OUTSIDE)
				{
					//���œ�����O�̏ꍇ
					if (m_loc == LOWER)
					{
						//m_position.v += C_PLAYER_RAD * 2;

						//LOWER�̍��W�w��
						m_outside_pos.v = m_position.v + C_PLAYER_RAD * 2;

					}
					//��œ�����O�̏ꍇ
					else
					{
						//m_position.v -= C_PLAYER_RAD * 2;

						//UPPER�̍��W�w��
						m_outside_pos.v = m_position.v - C_PLAYER_RAD * 2;
					}
					m_side = INSIDE;
				}
				else
				{
					//���ŊO������̏ꍇ
					//if (m_loc == LOWER) { m_position.v -= C_PLAYER_RAD * 0; }
					//��ŊO������̏ꍇ
					//else { m_position.v += C_PLAYER_RAD * 0; }
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

	//���O�ړ����o�p
	if (m_isChange)
	{
		//������O
		if (m_side == OUTSIDE)
		{
			m_outside_rad -= C_SUB_RAD;
			if (m_outside_rad == 0) { m_isChange = false; }
		}
		//�O�����
		else
		{
			m_outside_rad += C_SUB_RAD;
			if (m_outside_rad == C_PLAYER_RAD) { m_isChange = false; }
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
	DrawCircleAA(
		m_position.u + Shake::GetShake().u,
		m_position.v + Shake::GetShake().v,
		C_PLAYER_RAD - m_outside_rad,
		100,
		GetColor(13, 13, 13),
		true
	);
	//�O���p
	DrawCircleAA(
		m_outside_pos.u + Shake::GetShake().u,
		m_outside_pos.v + Shake::GetShake().v,
		m_outside_rad,
		100,
		GetColor(13, 13, 13),
		true
	);

	//���X�e�[�W
	float hoge = Shake::GetPowerX();
	DrawExtendGraph(640 + Shake::GetShake().u - m_stageSize.u / 2.0f, 360 + Shake::GetShake().v - m_stageSize.v / 2.0f,
					640 + Shake::GetShake().u + m_stageSize.u / 2.0f, 360 + Shake::GetShake().v + m_stageSize.v / 2.0f,
					m_s_stage, true);
	DrawFormatString(0, 40, GetColor(255, 255, 255), "ShakeX:%f", hoge);
	/*DrawCircle(
		640 + Shake::GetShake().u,
		360 + Shake::GetShake().v,
		C_STAGE_RAD,
		GetColor(255, 255, 255),
		false
	);*/
}

void Player::LoadFile()
{
	m_sprite = LoadGraph("Resources/particle.png");
	m_s_stage = LoadGraph("Resources/circle.png");
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