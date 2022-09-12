#include "Enemy.h"
#include "../Math/Math.h"
#include "../Lib/Lib.h"
#include "../Lib/Lib.h"
#include "../Math/Math.h"
#include "../Object/BaseObject.h"
#include "../Score/Score.h"
#include "../Particle/Particle.h"
#include "../Sound/Sound.h"

int BaseEnemy::m_sprite[7];
std::list<BaseEnemy*> EnemyManager::enemys;
std::list<std::list<BaseEnemy*>::iterator> EnemyManager::deleteEnemys;

FLOAT2 BaseEnemy::CiycleCenter = { WindowSize::Wid / 2, WindowSize::Hi / 2 };
float BaseEnemy:: TowerR = 100;
int BaseEnemy::SpornAngle = 45;

int enemyCiycle;

int EnemyManager::nowTowerR = MaxR;
int EnemyManager::nowCenterR = MaxR;
void BaseEnemy::LoadFile()
{
	m_sprite[FriendMode] = LoadGraph("Resources/score_enemy.png");
	m_sprite[ProgressMode] = LoadGraph("Resources/enemy.png");
	m_sprite[NormalMode] = LoadGraph("Resources/enemy.png");
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
	m_HP = MaxHP;
	m_size = {20 , 20};
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
		//HitShiled();
	}
	else if (m_state == ReturnCiycle)
	{
		ReturnToCiycle();
	}

	BulletCollision();

	if (m_state == ToCiycle)
	{
		nowSpriteNum = NormalMode;
	}
	else if (m_state == ToCenter && m_HP > 0)
	{
		nowSpriteNum = ProgressMode;
	}
	else if (m_state == ToCenter && m_HP <= 0)
	{
		nowSpriteNum = FriendMode;
	}

}

void BaseEnemy::Draw()
{
	DrawExtendGraph(m_position.u - (m_size.u / 2), m_position.v - (m_size.v / 2),
					m_position.u + (m_size.u / 2), m_position.v + (m_size.v / 2), m_sprite[nowSpriteNum], true);
	//DrawCircle(m_position.u, m_position.v,10,  GetColor(13, 13, 13));
}

void BaseEnemy::ToCiycleMove()
{
	m_easeTimer += (float)ToCenterSpeed / 200.0f;
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
	m_angle += OnCiycleSpeed / 10.0f;
	m_position.u = BaseEnemy::CiycleCenter.u + DxLibMath::Cos(m_angle) * CenterR;
	m_position.v = BaseEnemy::CiycleCenter.v + DxLibMath::Sin(m_angle) * CenterR;
	m_timer--;
	if (m_timer <= 0)
	{
		m_state = ToCenter;
	}
}

void BaseEnemy::ReturnToCiycle()
{
	m_easeTimer += (float)ToCenterSpeed / 80.0f;
	m_position = Easeing::EaseInQuad(m_position, m_endPosition, m_easeTimer);
	if (m_easeTimer >= 1.0f)
	{
		m_position.u = BaseEnemy::CiycleCenter.u + DxLibMath::Cos(m_angle) * CenterR;
		m_position.v = BaseEnemy::CiycleCenter.v + DxLibMath::Sin(m_angle) * CenterR;
		m_easeTimer = 0.0f;
		m_HP = MaxHP;
		m_state = ToCenter;
		m_isReturn = false;
	}
}

void BaseEnemy::LineMove()
{
	m_position = Easeing::EaseInQuad(m_position, BaseEnemy::CiycleCenter, (float)ToCenterSpeed / 50.0f);
	if (Collision::Lenght(BaseEnemy::CiycleCenter, m_position) < TowerR)
	{
		//スコア加算
		if (m_type == Angel)
		{
			int a = 0;
			a++;
			FLOAT2 size = { 18.0f, 22.0f };
			int score = 10 * (m_returnNum + 1);
			ParticleManager::scoreParitcle.AddScore(m_position, size, size, score, 60);
			StopSoundMem(SoundManager::addScore);
			PlaySoundMem(SoundManager::addScore, DX_PLAYTYPE_BACK);
		}
		//ライフで受ける
		else
		{
			int a = 0;
			a++;
			FLOAT2 size = { 18.0f, 22.0f };
			FLOAT2 start = {40.0f, 40.0f};
			FLOAT2 end = {0.0f, 0.0f};
			ParticleManager::pinkParticle.ExprotionParticle(m_position, start, end, 5, 30);
			//ParticleManager::scoreParitcle.AddScore(m_position, size, size, 99, 60);
			TowerHP::HP--;
		}
		isDelete = true;
	}
	
}

void BaseEnemy::HitShiled()
{
	if (true)
	{
		//以下反射板に当たった時
		m_isReturn = true;
		m_returnNum++;
		m_timer = MaxTimer;
		m_state = ReturnCiycle;
		float cos = DxLibMath::Cos(m_angle);
		float sin = DxLibMath::Sin(m_angle);
		m_endPosition = { BaseEnemy::CiycleCenter.u + CenterR * cos , BaseEnemy::CiycleCenter.v + CenterR * sin };
		m_easeTimer = 0.0f;
	}
	
}

void BaseEnemy::BulletCollision()
{
	for (auto itr = ObjectManager::object1.m_objects.begin(); itr != ObjectManager::object1.m_objects.end(); ++itr)
	{
		if (m_HP <= 0)continue;
		if ((*itr)->m_isHit) continue;
		if (Collision::CiycleCollision(m_position, 10, (*itr)->m_position, 10))
		{
			(*itr)->m_isHit = true;
			ObjectManager::object1.m_deleteObject.push_back(itr);
			m_HP--;
			m_timer = 0;
			//スコア加算
			Score::score++;
			FLOAT2 size = { 10.0f, 17.0f };
			ParticleManager::scoreParitcle.AddScore((*itr)->m_position, size , size, 1, 60);

			if (m_HP <= 0)
			{
				m_type = Angel;
			}

			StopSoundMem(SoundManager::shotHitSound);
			PlaySoundMem(SoundManager::shotHitSound, DX_PLAYTYPE_BACK);
		}
	}
	for (auto itr = ObjectManager::object2.m_objects.begin(); itr != ObjectManager::object2.m_objects.end(); ++itr)
	{
		if (m_HP <= 0)continue;
		if ((*itr)->m_isHit) continue;
		if (Collision::CiycleCollision(m_position, 10, (*itr)->m_position, 10))
		{
			(*itr)->m_isHit = true;
			ObjectManager::object2.m_deleteObject.push_back(itr);
			m_HP--;
			m_timer = 0;
			//スコア加算
			Score::score++;
			FLOAT2 size = { 10.0f, 17.0f };
			ParticleManager::scoreParitcle.AddScore((*itr)->m_position, size, size, 1, 60);

			if (m_HP <= 0)
			{
				m_type = Angel;
			}

			StopSoundMem(SoundManager::shotHitSound);
			PlaySoundMem(SoundManager::shotHitSound, DX_PLAYTYPE_BACK);
		}
	}
}

EnemyManager::EnemyManager()
{
	enemyCiycle = LoadGraph("Resources/spawn_circle.png");
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
		if ((*itr)->isDelete)
		{
			deleteEnemys.push_back(itr);
		}
	}
	for (auto itr = deleteEnemys.begin(); itr != deleteEnemys.end(); ++itr)
	{
		enemys.erase(*itr);
	}
	deleteEnemys.clear();
}

void EnemyManager::Draw()
{
	DrawCircleAA(WindowSize::Wid / 2, WindowSize::Hi / 2, nowCenterR, 128,  GetColor(200, 13, 13), 0, 1.0f);
	DrawCircleAA(WindowSize::Wid / 2, WindowSize::Hi / 2, nowTowerR, 128, GetColor(13, 200, 13), 0, 1.0f);
	for (auto itr = enemys.begin(); itr != enemys.end(); ++itr)
	{
		(*itr)->Draw();
	}
}

void EnemyManager::CiycleInc()
{
	nowTowerR = Easeing::EaseInQuad(nowTowerR, MaxR, 0.2f);
	nowCenterR = Easeing::EaseInQuad(nowCenterR, MaxR, 0.2f);
}

void EnemyManager::CiycleDec()
{
	nowTowerR = Easeing::EaseInQuad(nowTowerR, BaseEnemy::TowerR, 0.3f);
	nowCenterR = Easeing::EaseInQuad(nowCenterR, BaseEnemy::CenterR, 0.3f);

	/*nowTowerR = Easeing::EaseOutBounce(MaxR, BaseEnemy::TowerR, 0.3f);
	nowCenterR = Easeing::EaseOutBounce(MaxR, BaseEnemy::CenterR, 0.3f);*/
}

void EnemyManager::AllDelete()
{
	enemys.clear();
	deleteEnemys.clear();
}
