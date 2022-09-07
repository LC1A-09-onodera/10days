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

	//内外移動の処理
	if (!m_isMove)
	{
		if (Input::GetKeyTrigger(KEY_INPUT_SPACE) || Input::isJoyBottomTrigger(XINPUT_BUTTON_A))
		{
			//縦移動
			if (m_spaceCount >= 2)
			{
				m_spaceCount = 0;
				m_isMove = true;
			}
			//内外移動
			else
			{
				//内外移動演出用
				m_isChange = true;

				if (m_side == OUTSIDE)
				{
					//下で内から外の場合
					if (m_loc == LOWER)
					{
						//m_position.v += C_PLAYER_RAD * 2;

						//LOWERの座標指定
						m_outside_pos.v = m_position.v + C_PLAYER_RAD * 2;

					}
					//上で内から外の場合
					else
					{
						//m_position.v -= C_PLAYER_RAD * 2;

						//UPPERの座標指定
						m_outside_pos.v = m_position.v - C_PLAYER_RAD * 2;
					}
					m_side = INSIDE;
				}
				else
				{
					//下で外から内の場合
					//if (m_loc == LOWER) { m_position.v -= C_PLAYER_RAD * 0; }
					//上で外から内の場合
					//else { m_position.v += C_PLAYER_RAD * 0; }
					m_side = OUTSIDE;
				}
				m_spaceCount++;
			}
		}
	}

	//縦移動中の処理
	else
	{
		//加算処理
		if (m_easeTimer < 1.0f) { m_easeTimer += C_ADD_TIMER; }

		//下から上
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
		//上から下
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

	//内外移動演出用
	if (m_isChange)
	{
		//内から外
		if (m_side == OUTSIDE)
		{
			m_outside_rad -= C_SUB_RAD;
			if (m_outside_rad == 0) { m_isChange = false; }
		}
		//外から内
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

	//仮自機
	DrawCircleAA(
		m_position.u + Shake::GetShake().u,
		m_position.v + Shake::GetShake().v,
		C_PLAYER_RAD - m_outside_rad,
		100,
		GetColor(13, 13, 13),
		true
	);
	//外側用
	DrawCircleAA(
		m_outside_pos.u + Shake::GetShake().u,
		m_outside_pos.v + Shake::GetShake().v,
		m_outside_rad,
		100,
		GetColor(13, 13, 13),
		true
	);

	//仮ステージ
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