#include "UI.h"
#include "../Lib/Lib.h"

void BulletUI::AddBullet()
{
	UISprite* bullet = new UISprite();
	bullet->m_position = { -100, 720 / 2 };
	float sen = 0.1f;
	bullet->m_size = { 167.0f * sen, 70.0f * sen };
	m_bullets.push_back(bullet);
}

void BulletUI::Update(int bulletNum)
{
	int nowBullet = m_bullets.size();
	if (nowBullet < bulletNum)
	{
		AddBullet();
	}
	if (nowBullet > bulletNum)
	{
		ShotBullet();
		nowBullet = m_bullets.size();
	}
	if (nowBullet % 2 == 0)
	{
		standardPositionY = 720 / 2 + bulletDistance / 2;
		auto itr = m_bullets.begin();
		for (int i = 0; i < nowBullet / 2; i++)
		{
			(*itr)->m_endPos = { (float)Xposition , standardPositionY - (float)bulletDistance * (nowBullet / 2 - i) };
			itr++;
		}
		for (int i = 0; i < nowBullet / 2; i++)
		{
			(*itr)->m_endPos = { (float)Xposition , standardPositionY + (float)bulletDistance * (i) };
			itr++;
		}
	}
	else
	{
		standardPositionY = 720 / 2;
		auto itr = m_bullets.begin();
		for (int i = 0; i < (nowBullet - 1) / 2; i++)
		{
			(*itr)->m_endPos = { (float)Xposition , standardPositionY - (float)bulletDistance * ((nowBullet - 1) / 2 - i) };
			itr++;
		}
		(*itr)->m_endPos = { (float)Xposition , (float)standardPositionY };
		itr++;
		for (int i = 0; i < (nowBullet - 1) / 2; i++)
		{
			(*itr)->m_endPos = { (float)Xposition , standardPositionY + (float)bulletDistance * (i + 1) };
			itr++;
		}
	}
	if (m_bullets.size() > 0)
	{
		for (auto itr = m_bullets.begin(); itr != m_bullets.end(); ++itr)
		{
			(*itr)->EaseMove();
		}
	}
}

void BulletUI::Draw()
{
	if (m_bullets.size() > 0)
	{
		for (auto itr = m_bullets.begin(); itr != m_bullets.end(); ++itr)
		{
			DrawExtendGraph((*itr)->m_position.u - (*itr)->m_size.u / 2, (*itr)->m_position.v - (*itr)->m_size.v / 2,
				(*itr)->m_position.u + (*itr)->m_size.u / 2, (*itr)->m_position.v + (*itr)->m_size.v / 2, m_sprite, true);
		}
	}
}

void BulletUI::LoadFile()
{
	m_sprite = LoadGraph("Resources/bullet_sq.png");
}

void BulletUI::ShotBullet()
{
	auto itr = m_bullets.begin();
	m_bullets.erase(itr);
}

void UISprite::EaseMove()
{
	m_position = Easeing::EaseInQuad(m_position, m_endPos, easeSpeed);
}
