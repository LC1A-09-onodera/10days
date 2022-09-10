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

	m_start_pos = { 0,0 };
	m_vec = { 0,0 };
	m_end_pos = { 0,0 };
	m_stage_rad = C_STAGE_RAD;
	m_bulletNum = C_BULLET_INIT_VAL;
	m_maxBulletNum = m_bulletNum;
	m_stage_rad = C_STAGE_RAD;
	m_easeTimer = 0.0f;
	m_deg = 0;
	m_isMove = false;
	m_stageSize = { 504, 504 };
	m_isReload = false;
}

void Player::Update()
{
	AddForce();

	AttachForce();

	if (m_bulletNum == 0)
	{
		m_bulletNum = m_maxBulletNum;
	}

	//通常時
	if (!m_isMove)
	{
		//自機の方向ベクトルを計算
		FLOAT2 l_diff = { 0,0 };
		l_diff.u = m_position.u - C_HALF_WID;
		l_diff.v = m_position.v - C_HALF_HEI;
		float l_len = sqrtf(
			powf(l_diff.u, 2.0f) +
			powf(l_diff.v, 2.0f));
		FLOAT2 l_vec = { 0,0 };
		l_vec.u = l_diff.u / l_len;
		l_vec.v = l_diff.v / l_len;
		m_deg = 180.0f / DX_PI_F * atan2f(-l_vec.v, -l_vec.u);

		//左スティックが倒されている時のみ(コントローラー以外も対応させろ！)
		if (Input::isJoyLeftStickBottom())
		{
			//自機の位置算出正規化
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
			//要修正
			float l_rad = (l_pAngle + (l_nearArc / 30.0f)) * DX_PI_F / 180.0f;
			l_nearVec.u = cosf(l_rad);
			l_nearVec.v = sinf(l_rad);

			m_position.u = l_nearVec.u * C_STAGE_RAD + C_HALF_WID;
			m_position.v = l_nearVec.v * C_STAGE_RAD + C_HALF_HEI;
		}

		//縦断入力
		if (Input::GetKeyTrigger(KEY_INPUT_SPACE) ||
			Input::isJoyBottomTrigger(XINPUT_BUTTON_A))
		{
			m_bulletNum = m_maxBulletNum;
			m_start_pos = m_position;
			l_vec.u *= -1.0f;
			l_vec.v *= -1.0f;
			m_end_pos.u = l_vec.u * C_STAGE_RAD + C_HALF_WID;
			m_end_pos.v = l_vec.v * C_STAGE_RAD + C_HALF_HEI;
			m_vec = l_vec;
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
	DrawCircleAA(
		m_position.u + Shake::GetShake().u,
		m_position.v + Shake::GetShake().v,
		C_PLAYER_RAD,
		100,
		GetColor(13, 13, 13),
		true
	);

	//仮ステージ
	DrawExtendGraph(
		(WindowSize::Wid / 2) + Shake::GetShake().u - m_stageSize.u / 2.0f,
		(WindowSize::Hi / 2) + Shake::GetShake().v - m_stageSize.v / 2.0f,
		(WindowSize::Wid / 2) + Shake::GetShake().u + m_stageSize.u / 2.0f,
		(WindowSize::Hi / 2) + Shake::GetShake().v + m_stageSize.v / 2.0f,
		m_s_stage,
		true
	);

	float hoge = Shake::GetPowerX();
	DrawFormatString(0, 40, GetColor(0, 0, 0), "ShakeX:%f", hoge);
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
