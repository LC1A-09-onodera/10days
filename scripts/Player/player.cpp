#include "player.h"
#include "../Input/Input.h"

void Player::Init()
{
	m_position = {0.0f, 0.0f};
	m_gravity.v = 0.75f;
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
	if (Input::GetKeyTrigger(KEY_INPUT_D))
	{
		m_addForce.u = 15.0f;
		m_addForce.v = -10.0f;
	}
	if (Input::GetKeyTrigger(KEY_INPUT_A))
	{
		m_addForce.u = -15.0f;
		m_addForce.v = -10.0f;
	}
}

void Player::AttachForce()
{
	m_position.u += m_addForce.u;
	m_position.v += m_addForce.v;
	m_addForce.u = m_addForce.u / 1.1f;
	m_addForce.v += m_gravity.v;
}

