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
	m_start_pos = { 0,0 };
	m_vec = { 0,0 };
	m_end_pos = { 0,0 };
	m_spaceCount = 2;
	m_stage_rad = C_STAGE_RAD;
	m_bulletNum = C_BULLET_INIT_VAL;
	m_maxBulletNum = m_bulletNum;
	m_stage_rad = C_STAGE_RAD;
	m_easeTimer = 0.0f;
	m_outside_rad = 0.0f;
	m_side = OUTSIDE;
	m_loc = LOWER;
	m_isMove = false;
	m_stageSize = { 504, 504 };
	m_isChange = false;
	m_isChangeTrigger = false;
	m_isReload = false;
}

void Player::Update()
{
	AddForce();

	AttachForce();

	//通常時
	if (!m_isMove)
	{
		//左スティックが倒されている時のみ(コントローラー以外も対応させろ！)
		if (Input::isJoyLeftStickBottom())
		{
			//自機の位置算出正規化
			m_vec = Input::GetJoyLeftStick();
			float l_len = sqrtf(powf(m_vec.u, 2.0f) + powf(m_vec.v, 2.0f));
			m_vec.u /= l_len;
			m_vec.v /= l_len;
			m_position.u = m_vec.u * C_STAGE_RAD + C_HALF_WID;
			m_position.v = -m_vec.v * C_STAGE_RAD + C_HALF_HEI;
		}

		//縦断入力
		if (Input::GetKeyTrigger(KEY_INPUT_SPACE) ||
			Input::isJoyBottomTrigger(XINPUT_BUTTON_A))
		{
			m_bulletNum = m_maxBulletNum;
			m_start_pos = m_position;
			m_end_pos.u = -m_vec.u * C_STAGE_RAD + C_HALF_WID;
			m_end_pos.v = m_vec.v * C_STAGE_RAD + C_HALF_HEI;
			m_isMove = true;
		}
	}

	//縦断移動中
	else
	{
		//タイマー加算
		if (m_easeTimer < 1.0f) { m_easeTimer += C_ADD_TIMER; }

		//移動処理
		m_position.u = (m_end_pos.u - m_start_pos.u) * easeInOutSine(m_easeTimer) + m_start_pos.u;
		m_position.v = (m_end_pos.v - m_start_pos.v) * easeInOutSine(m_easeTimer) + m_start_pos.v;
		if (m_easeTimer >= 1.0f)
		{
			m_easeTimer = 0.0f;
			m_isMove = false;
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
	if (m_outside_rad < C_PLAYER_RAD)
	{
		DrawCircleAA(
			m_position.u + Shake::GetShake().u,
			m_position.v + Shake::GetShake().v,
			C_PLAYER_RAD - m_outside_rad,
			100,
			GetColor(13, 13, 13),
			true
		);
	}
	//外側用
	if (m_outside_rad > 0)
	{
		DrawCircleAA(
			m_outside_pos.u + Shake::GetShake().u,
			m_outside_pos.v + Shake::GetShake().v,
			m_outside_rad,
			100,
			GetColor(13, 13, 13),
			true
		);
	}

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

bool Player::ShotBullet()
{
	if (m_bulletNum > 0)
	{
		m_bulletNum--;
		return true;
	}
	return false;
}
