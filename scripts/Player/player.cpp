#include "player.h"
#include "../Input/Input.h"
#include "../Scroll/Scroll.h"

void Player::Init()
{
	m_position = { 0.0f, 0.0f };
	m_gravity.v = 0.9f;
	m_scrollStartLine = { m_winSize.u / 2, 0 };
}

void Player::Update()
{
	AddForce();

	AttachForce();

	runParticle.Update();
}

void Player::Draw()
{
	DrawGraph(static_cast<int>(m_position.u) - Scroll::GetScrollX(), static_cast<int>(m_position.v), m_sprite, true);
	runParticle.Draw();

	DrawFormatString(0, 0, GetColor(255, 255, 255), "posX:%2f, posY:%2f", m_position.u, m_position.v);
}

void Player::LoadFile()
{
	m_sprite = LoadGraph("Resources/particle.png");
	runParticle.LoadFile("Resources/particle.png");

	Init();
}

void Player::AddForce()
{
	float sen = 2.0f;
	if (Input::GetKeyTrigger(KEY_INPUT_D))
	{
		m_addForce.u = 15.0f * sen;
		m_addForce.v = -25.0f * sen;
	}
	else if (Input::GetKey(KEY_INPUT_D))
	{
		m_addForce.u = 10.0f * sen;
		m_addForce.v = -3.5f;
	}
	if (Input::GetKeyTrigger(KEY_INPUT_A))
	{
		m_addForce.u = -15.0f * sen;
		m_addForce.v = -25.0f * sen;
	}
	else if (Input::GetKey(KEY_INPUT_A))
	{
		m_addForce.u = -10.0f * sen;
		m_addForce.v = -3.5f;
	}
}

void Player::AttachForce()
{
	m_position.u += m_addForce.u;
	m_position.v += m_addForce.v;
	m_addForce.u = m_addForce.u / 1.1f;
	m_addForce.v += m_gravity.v;
	//地面につく処理
	if (m_position.v + m_size.v > m_winSize.v)
	{
		m_position.v = m_winSize.v - m_size.v;
		m_addForce.v = 0;
		m_addForce.u = 0;
	}
	//左に行かないように
	if (m_position.u < 0)
	{
		m_addForce.u = 0;
		m_position.u = 1.0f;
	}
	//スクロールしない時に右に当たる処理だったもの
	/*else if (m_position.u + m_size.u > m_winSize.u + Scroll::GetScrollX())
	{
		m_position.u = m_winSize.u - m_size.u;
	}*/
	//真ん中に行ったらスクロール開始
	if (m_scrollStartLine.u - (m_size.u / 2) < m_position.u)
	{
		FLOAT2 scrollVal = { 0 };
		scrollVal.u = m_position.u - m_scrollStartLine.u + (m_size.u / 2) - Scroll::GetScrollX();
		Scroll::AddSCroll(scrollVal);
	}
	//パーティクルの生成
	if (abs(m_addForce.v) > 1 || abs(m_addForce.u) > 1)
	{
		FLOAT2 startSize = { 20.0f, 20.0f };
		FLOAT2 endSize = { 0.0f, 0.0f };
		FLOAT2 emit = { 0 };
		if (m_addForce.u > 0)
		{
			emit = { m_position.u, m_position.v + m_size.v / 2 };
		}
		else
		{
			emit = { m_position.u + m_size.u, m_position.v + m_size.v / 2 };
		}
		runParticle.FlyParticle(emit, startSize, endSize, 5, 60);
	}
}
