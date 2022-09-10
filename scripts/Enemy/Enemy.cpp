#include "Enemy.h"
#include "../Math/Math.h"
#include "../Lib/Lib.h"
#include "../Lib/Lib.h"
#include "../Math/Math.h"

int BaseEnemy::m_sprite[7];
std::list<BaseEnemy*> EnemyManager::enemys;

FLOAT2 BaseEnemy::CiycleCenter = { WindowSize::Wid / 2, WindowSize::Hi / 2 };
float BaseEnemy:: TowerR = 100;
int BaseEnemy::SpornAngle = 45;

void BaseEnemy::LoadFile()
{
	for (int i = 0;i < 5;i++)
	{
		m_sprite[i] = LoadGraph("Resources/.png");
	}
}

void BaseEnemy::Init()
{
	m_angle = (float)SpornAngle;
	float cos = DxLibMath::Cos(m_angle);
	float sin = DxLibMath::Sin(m_angle);
	
	m_position.u = BaseEnemy::CiycleCenter.u + OutsideR * cos;
	m_position.v = BaseEnemy::CiycleCenter.v + OutsideR * sin;
	m_endPosition.u = BaseEnemy::CiycleCenter.u + CenterR * cos;
	m_endPosition.v = BaseEnemy::CiycleCenter.v + CenterR * sin;
	SpornAngle = SpornAngle + 120 + (rand() % 60);
	m_timer = MaxTimer;
	m_easeTimer = 0.0f;
}

void BaseEnemy::Update()
{
	if (m_state == ToCiycle)
	{
		ToCiycleMove();
	}
	else if (m_state == OnCiycle)
	{
		CiycleMove();
	}
	else if (m_state == ToCenter)
	{
		LineMove();
	}

	//if ()
}

void BaseEnemy::Draw()
{
	/*DrawExtendGraph(m_position.u - (m_size.u / 2), m_position.v - (m_size.v / 2),
					m_position.u + (m_size.u / 2), m_position.v + (m_size.v / 2), m_sprite[nowSpriteNum], true);*/
	DrawCircle(m_position.u, m_position.v,10,  GetColor(13, 13, 13));
}

void BaseEnemy::ToCiycleMove()
{
	m_easeTimer += (float)ToCenterSpeed / 80.0f;
 	m_position = Easeing::EaseInQuad(m_position, m_endPosition, m_easeTimer);
	if (m_easeTimer >= 1.0f)
	{
		m_position.u = BaseEnemy::CiycleCenter.u + DxLibMath::Cos(m_angle) * CenterR;
		m_position.v = BaseEnemy::CiycleCenter.v + DxLibMath::Sin(m_angle) * CenterR;
		m_state = OnCiycle;
	}
}

void BaseEnemy::CiycleMove()
{
	m_angle += OnCiycleSpeed;
	m_position.u = BaseEnemy::CiycleCenter.u + DxLibMath::Cos(m_angle) * CenterR;
	m_position.v = BaseEnemy::CiycleCenter.v + DxLibMath::Sin(m_angle) * CenterR;
	m_timer--;
	if (m_timer <= 0)
	{
		m_state = ToCenter;
	}
}

void BaseEnemy::LineMove()
{
	m_position = Easeing::EaseInQuad(m_position, BaseEnemy::CiycleCenter, (float)ToCenterSpeed / 10.0f);
	if (Collision::Lenght(BaseEnemy::CiycleCenter, m_position) < TowerR)
	{
		//スコア加算
		if (m_type == Angel)
		{
			int a = 0;
			a++;
		}
		//ライフで受ける
		else
		{
			int a = 0;
			a++;
		}
	}
	
}

void EnemyManager::Init()
{
	
}

void EnemyManager::AddEnemy()
{
	BaseEnemy *enemy = new BaseEnemy();
	enemy->Init();
	EnemyManager::enemys.push_back(&(*enemy));
}

void EnemyManager::Update()
{
	for (auto itr = enemys.begin(); itr != enemys.end(); ++itr)
	{
		(*itr)->Update();
	}
}

void EnemyManager::Draw()
{
	for (auto itr = enemys.begin(); itr != enemys.end(); ++itr)
	{
		(*itr)->Draw();
	}
}
