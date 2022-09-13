#include "UI.h"
#include "../Lib/Lib.h"
#include "../WindowsSize/WindowSize.h"
#include "../Math/Math.h"
#include "../Sound/Sound.h"

using namespace DxLibMath;


std::list<UISprite*> ScoreUI::m_scores;
int ScoreUI::s_numbers[10];
int ScoreUI::nowScore;
float ScoreUI::ext = 0.1f;

void BulletUI::AddBullet()
{
	UISprite* bullet = new UISprite();
	bullet->m_position = { -100, WindowSize::Hi / 2 };
	float sen = 0.1f;
	bullet->m_size = { 167.0f * sen, 70.0f * sen };
	m_bullets.push_back(bullet);
}

void BulletUI::AddBullet2()
{
	UISprite* bullet = new UISprite();
	bullet->m_position = { WindowSize::Wid / 2, WindowSize::Hi / 2 };
	bullet->m_endPos = { WindowSize::Wid / 2 + BulletR * Cos(spornAngle), WindowSize::Hi / 2 + BulletR * Sin(spornAngle) };
	float sen = 0.1f;
	bullet->m_size = { 167.0f * sen, 70.0f * sen };
	m_bullets.push_back(bullet);
	spornAngle += 5.0f;
}

void BulletUI::Update(int bulletNum)
{
	int nowBullet = m_bullets.size();
	if (m_isAllShot)
	{
		AllShot();
		nowBullet = m_bullets.size();
	}
	else if (nowBullet < bulletNum)
	{
		for (int i = 0l; i < 3; i++)
		{
			if (nowBullet < bulletNum)
			{
				AddBullet2();
				nowBullet = m_bullets.size();
			}
		}
	}
	if (nowBullet > bulletNum)
	{
		ShotBullet();
		nowBullet = m_bullets.size();
	}

	//Update1();
	Update2();

	if (m_bullets.size() > 0)
	{
		for (auto itr = m_bullets.begin(); itr != m_bullets.end(); ++itr)
		{
			(*itr)->EaseMove();
		}
	}
}

void BulletUI::Update1()
{
	int nowBullet = m_bullets.size();
	if (nowBullet % 2 == 0)
	{
		standardPositionY = WindowSize::Hi / 2 + bulletDistance / 2;
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
		standardPositionY = WindowSize::Hi / 2;
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
}

void BulletUI::Update2()
{
	float angle = 0;
	for (auto itr = m_bullets.begin(); itr != m_bullets.end(); ++itr)
	{
		(*itr)->m_endPos = { WindowSize::Wid / 2 + BulletR * Cos(angle + 180.0f), WindowSize::Hi / 2 + BulletR * Sin(angle + 180.0f) };
		(*itr)->m_position = Easeing::EaseInQuad((*itr)->m_position, (*itr)->m_endPos, 0.3f);
		angle += 5.0f;
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
	spornAngle -= 5.0f;
}

void BulletUI::AllShotStart()
{
	m_isAllShot = true;
}

void BulletUI::AllShot()
{
	if (m_bullets.size() > 0)
	{
		ShotBullet();
	}
}

int BulletUI::BulletNum()
{
	return m_bullets.size();
}

bool BulletUI::GetIsAllShot()
{
	return m_isAllShot;
}

void UISprite::EaseMove()
{
	m_position = Easeing::EaseInQuad(m_position, m_endPos, easeSpeed);
}

void HPUI::Draw()
{
	if (m_HPs.size() > 0)
	{
		for (auto itr = m_HPs.begin(); itr != m_HPs.end(); ++itr)
		{
			DrawExtendGraph((*itr)->m_position.u - (*itr)->m_size.u / 2, (*itr)->m_position.v - (*itr)->m_size.v / 2,
				(*itr)->m_position.u + (*itr)->m_size.u / 2, (*itr)->m_position.v + (*itr)->m_size.v / 2, m_sprite, true);
		}
	}
}

void ScoreUI::LoadFile()
{
	LoadDivGraph("Resources/numbers.png", 10, 5, 2, 700 / 5, 346 / 2, s_numbers);
}

void ScoreUI::Init()
{
	for (int i = 0; i < 6; i++)
	{
		UISprite* ui = new UISprite();
		m_scores.push_back(ui);
	}
}

void ScoreUI::Update(int score)
{
	float startAngle = 160;
	nowScore = score;
	for (auto itr = m_scores.begin(); itr != m_scores.end(); ++itr)
	{
		(*itr)->m_position.u = WindowSize::Wid / 2 + (R * DxLibMath::Cos(startAngle));
		(*itr)->m_position.v = WindowSize::Hi / 2 + (R * DxLibMath::Sin(startAngle));
		//(*itr)->angle = startAngle - 180.0f;
		startAngle -= 28;
	}
}

void ScoreUI::Draw()
{
	int hoge = 100000;
	auto itr = m_scores.begin();
	DrawRotaGraph((*itr)->m_position.u, (*itr)->m_position.v, ext, 20, s_numbers[nowScore / 100000 % 10], true);
	itr++;
	DrawRotaGraph((*itr)->m_position.u, (*itr)->m_position.v, ext, 48, s_numbers[nowScore / 10000 % 10], true);
	itr++;
	DrawRotaGraph((*itr)->m_position.u, (*itr)->m_position.v, ext, 0, s_numbers[nowScore / 1000 % 10], true);
	itr++;
	DrawRotaGraph((*itr)->m_position.u, (*itr)->m_position.v, ext, 0, s_numbers[nowScore / 100 % 10], true);
	itr++;
	DrawRotaGraph((*itr)->m_position.u, (*itr)->m_position.v, ext, -48, s_numbers[nowScore / 10 % 10], true);
	itr++;
	DrawRotaGraph((*itr)->m_position.u, (*itr)->m_position.v, ext, -20, s_numbers[nowScore / 1 % 10], true);
}
