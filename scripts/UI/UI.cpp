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
FLOAT2 ScoreUI::scorePos[5];
int ScoreUI::s_score[5];

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
	m_sprite = LoadGraph("Resources/new_bullet_alpha.png");
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
	LoadDivGraph("Resources/numbers_alpha.png", 10, 5, 2, 690 / 5, 368 / 2, s_numbers);
	s_score[0] = LoadGraph("Resources/s.png");
	s_score[1] = LoadGraph("Resources/c.png");
	s_score[2] = LoadGraph("Resources/o.png");
	s_score[3] = LoadGraph("Resources/r.png");
	s_score[4] = LoadGraph("Resources/e.png");
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
	float startAngle = 135;
	nowScore = score;
	for (auto itr = m_scores.begin(); itr != m_scores.end(); ++itr)
	{
		(*itr)->m_position.u = WindowSize::Wid / 2 + (R * DxLibMath::Cos(startAngle));
		(*itr)->m_position.v = WindowSize::Hi / 2 + (R * DxLibMath::Sin(startAngle));
		//(*itr)->angle = startAngle - 180.0f;
		startAngle -= 18;
	}

	float scoreStart = 230;
	for (int i = 0; i < 5; i++)
	{
		scorePos[i].u = WindowSize::Wid / 2 + (R * DxLibMath::Cos(scoreStart));
		scorePos[i].v = WindowSize::Hi / 2 + (R * DxLibMath::Sin(scoreStart));
		//(*itr)->angle = startAngle - 180.0f;
		scoreStart += 20;
	}
}

void ScoreUI::Draw()
{
	int hoge = 100000;
	auto itr = m_scores.begin();
	FLOAT2 vec = { 0 };
	vec.u = WindowSize::Wid / 2 - (*itr)->m_position.u;
	vec.v = WindowSize::Hi / 2 - (*itr)->m_position.v;
	vec = Collision::Normalize(vec);
	float angle = atan2(vec.v, vec.u);
	angle += 3.141592f / 2.0f;
	DrawRotaGraph((*itr)->m_position.u, (*itr)->m_position.v, ext, angle, s_numbers[nowScore / 100000 % 10], true);
	itr++;
	vec.u = WindowSize::Wid / 2 - (*itr)->m_position.u;
	vec.v = WindowSize::Hi / 2 - (*itr)->m_position.v;
	vec = Collision::Normalize(vec);
	angle = atan2(vec.v, vec.u);
	angle += 3.141592f / 2.0f;
	DrawRotaGraph((*itr)->m_position.u, (*itr)->m_position.v, ext, angle, s_numbers[nowScore / 10000 % 10], true);
	itr++;
	vec.u = WindowSize::Wid / 2 - (*itr)->m_position.u;
	vec.v = WindowSize::Hi / 2 - (*itr)->m_position.v;
	vec = Collision::Normalize(vec);
	angle = atan2(vec.v, vec.u);
	angle += 3.141592f / 2.0f;
	DrawRotaGraph((*itr)->m_position.u, (*itr)->m_position.v, ext, angle, s_numbers[nowScore / 1000 % 10], true);
	itr++;
	vec.u = WindowSize::Wid / 2 - (*itr)->m_position.u;
	vec.v = WindowSize::Hi / 2 - (*itr)->m_position.v;
	vec = Collision::Normalize(vec);
	angle = atan2(vec.v, vec.u);
	angle += 3.141592f / 2.0f;
	DrawRotaGraph((*itr)->m_position.u, (*itr)->m_position.v, ext, angle, s_numbers[nowScore / 100 % 10], true);
	itr++;
	vec.u = WindowSize::Wid / 2 - (*itr)->m_position.u;
	vec.v = WindowSize::Hi / 2 - (*itr)->m_position.v;
	vec = Collision::Normalize(vec);
	angle = atan2(vec.v, vec.u);
	angle += 3.141592f / 2.0f;
	DrawRotaGraph((*itr)->m_position.u, (*itr)->m_position.v, ext, angle, s_numbers[nowScore / 10 % 10], true);
	itr++;
	vec.u = WindowSize::Wid / 2 - (*itr)->m_position.u;
	vec.v = WindowSize::Hi / 2 - (*itr)->m_position.v;
	vec = Collision::Normalize(vec);
	angle = atan2(vec.v, vec.u);
	angle += 3.141592f / 2.0f;
	DrawRotaGraph((*itr)->m_position.u, (*itr)->m_position.v, ext, angle, s_numbers[nowScore / 1 % 10], true);

	for (int i = 0; i < 5; i++)
	{
		vec.u = WindowSize::Wid / 2 - scorePos[i].u;
		vec.v = WindowSize::Hi / 2 - scorePos[i].v;
		vec = Collision::Normalize(vec);
		angle = atan2(vec.v, vec.u);
		angle -= 3.141592f / 2.0f;
		DrawRotaGraph(scorePos[i].u, scorePos[i].v, 0.3f, angle, s_score[i], true);
	}
}
