#include "player.h"
#include "../Input/Input.h"

void Player::Init()
{
	m_position = {0.0f, 0.0f};
	m_gravity.v = 0.9f;
}

void Player::Update()
{
	AddForce();

	AttachForce();
}

void Player::Draw()
{
	DrawGraph(static_cast<int>(m_position.u), static_cast<int>(m_position.v), m_sprite, true);
}

void Player::LoadFile()
{
	m_sprite = LoadGraph("Resources/particle.png");
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
	if (m_position.v + m_size.v > m_winSize.v)
	{
		m_position.v = m_winSize.v - m_size.v;
		m_addForce.v = 0;
		m_addForce.u = 0;
	}
	if (m_position.u < 0)
	{
		m_position.u = 1.0f;
	}
	else if (m_position.u + m_size.u > m_winSize.u)
	{
		m_position.u = m_winSize.u - m_size.u;
	}
}

