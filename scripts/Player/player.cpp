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
}

void Player::Draw()
{
	DrawGraph(static_cast<int>(m_position.u) - Scroll::GetScrollX(), static_cast<int>(m_position.v), m_sprite, true);


	DrawFormatString(0, 0, GetColor(255, 255, 255), "posX:%2f, posY:%2f", m_position.u, m_position.v);
}

void Player::LoadFile()
{
	m_sprite = LoadGraph("Resources/particle.png");

	Init();
}

void Player::AddForce()
{
	
}

void Player::AttachForce()
{
	
}
