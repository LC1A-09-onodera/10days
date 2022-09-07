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
