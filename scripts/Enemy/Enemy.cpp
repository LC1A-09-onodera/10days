#include "Enemy.h"
#include "../Math/Math.h"
#include "../Lib/Lib.h"
#include "../Lib/Lib.h"
#include "../Math/Math.h"
#include "../Object/BaseObject.h"
#include "../Score/Score.h"
#include "../Particle/Particle.h"
#include "../Sound/Sound.h"
#include "../Scroll/Scroll.h"
#include "../Player/player.h"

int BaseEnemy::m_sprite[4];
std::list<BaseEnemy*> EnemyManager::enemys;
std::list<std::list<BaseEnemy*>::iterator> EnemyManager::deleteEnemys;

FLOAT2 BaseEnemy::CiycleCenter = { WindowSize::Wid / 2, WindowSize::Hi / 2 };
float BaseEnemy::TowerR = 65;
int BaseEnemy::SpornAngle = 45;

int enemyCiycle;

int EnemyManager::nowTowerR = MaxR;
int EnemyManager::nowCenterR = MaxR;

void BaseEnemy::LoadFile()
{
	m_sprite[Normal] = LoadGraph("Resources/enemy_0.png");
	m_sprite[Midl] = LoadGraph("Resources/enemy_1.png");
	m_sprite[Hi] = LoadGraph("Resources/enemy_2.png");
	m_sprite[Bomb] = LoadGraph("Resources/bomb.png");
}

void BaseEnemy::Init(SpeedType type)
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
	m_size = { 50 , 50 };
	speedType = type;
	if (type == SpeedType::Normal)
	{
		m_ToCenterSpeed = 3.0f;
	}
	else if (type == SpeedType::Midl)
	{
		m_ToCenterSpeed = 4.0f;
	}
	else if (type == SpeedType::Hi)
	{
		m_ToCenterSpeed = 5.0f;
	}
	else if (type == SpeedType::Bomb)
	{
		m_HP = 10;
		m_timer = BombLife;
	}
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
	else if (m_state == ToCenter && speedType != SpeedType::Bomb)
	{
		LineMove();
		//HitShiled();
	}
	else if (speedType == SpeedType::Bomb && isBombErase)
	{
		m_ext = Easeing::EaseInQuad(m_ext, 0, 0.3f);
		if (m_ext <= 0.01f)
		{
			isDelete = true;
		}
	}
	/*else if (m_state == ReturnCiycle)
	{
		ReturnToCiycle();
	}*/

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
	/*DrawExtendGraph(m_position.u - (m_size.u / 2), m_position.v - (m_size.v / 2),
		m_position.u + (m_size.u / 2), m_position.v + (m_size.v / 2), m_sprite[speedType], true);*/
		//DrawCircle(m_position.u, m_position.v,10,  GetColor(13, 13, 13));
	FLOAT2 vec = { 0 };
	vec.u = WindowSize::Wid / 2 - m_position.u;
	vec.v = WindowSize::Hi / 2 - m_position.v;
	vec = Collision::Normalize(vec);
	float angle = atan2(vec.v, vec.u);
	if (speedType == SpeedType::Hi)
	{
		angle += 3.141582f / 2.0f;
	}
	if (speedType != SpeedType::Bomb)
	{
		DrawRotaGraph(m_position.u + Shake::GetShake().u + shakePower.u, m_position.v + Shake::GetShake().v + shakePower.v, m_ext, angle, m_sprite[speedType], true);
	}
	else
	{
		DrawRotaGraph(m_position.u + Shake::GetShake().u + shakePower.u, m_position.v + Shake::GetShake().v + shakePower.v, m_ext, angle, m_sprite[Bomb], true);
	}
}

void BaseEnemy::ToCiycleMove()
{
	m_easeTimer += (float)m_ToCenterSpeed / 200.0f;
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
		if (speedType == Bomb)
		{
			isBombErase = true;
		}
	}
	if (m_timer <= ShakeStartTime)
	{
		shakePower = { rand() % 6 - 2.0f, rand() % 6 - 2.0f };
	}
}

void BaseEnemy::ReturnToCiycle()
{
	m_easeTimer += (float)m_ToCenterSpeed / 50.0f;
	m_position = Easeing::EaseInQuad(m_position, m_endPosition, m_easeTimer);
	if (m_easeTimer >= 1.0f)
	{
		m_position.u = BaseEnemy::CiycleCenter.u + DxLibMath::Cos(m_angle) * CenterR;
		m_position.v = BaseEnemy::CiycleCenter.v + DxLibMath::Sin(m_angle) * CenterR;
		m_easeTimer = 0.0f;
		m_HP = (MaxHP - m_returnNum) < 0 ? 1 : MaxHP - m_returnNum;
		m_state = ToCenter;
		m_isReturn = false;
	}
}

void BaseEnemy::LineMove()
{
	if (isStop)
	{
		m_stopTimer++;
		if (m_stopTimer < StopTime)
		{
			return;
		}
		isStop = false;
	}
	m_position = Easeing::EaseInQuad(m_position, BaseEnemy::CiycleCenter, (float)m_ToCenterSpeed / 50.0f);
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
			Score::score += score;
			StopSoundMem(SoundManager::addScore);
			PlaySoundMem(SoundManager::addScore, DX_PLAYTYPE_BACK);
		}
		//ライフで受ける
		else
		{
			int a = 0;
			a++;
			FLOAT2 size = { 18.0f, 22.0f };
			FLOAT2 start = { 40.0f, 40.0f };
			FLOAT2 end = { 0.0f, 0.0f };
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
		m_HP = 0;
		isDelete = true;
		/*float addScore = 10;
		Score::score += addScore;
		FLOAT2 size = { 18.0f, 22.0f };
		ParticleManager::scoreParitcle.AddScore(m_position, size, size, addScore, 60);*/
		//以下反射板に当たった時
		m_isReturn = true;
		if (m_type == Angel)
		{
			m_returnNum++;
		}
		m_timer = MaxTimer;
		m_state = ReturnCiycle;
		float cos = DxLibMath::Cos(m_angle);
		float sin = DxLibMath::Sin(m_angle);
		m_endPosition = { BaseEnemy::CiycleCenter.u + CenterR * cos , BaseEnemy::CiycleCenter.v + CenterR * sin };
		m_easeTimer = 0.0f;
		m_ToCenterSpeed += 0.5f;
	}
}

void BaseEnemy::BulletCollision()
{
	for (auto itr = ObjectManager::object1.m_objects.begin(); itr != ObjectManager::object1.m_objects.end(); ++itr)
	{
		if (m_HP <= 0)continue;
		if ((*itr)->m_isHit) continue;
		if (Collision::CiycleCollision(m_position, 20, (*itr)->m_position, 10))
		{
			(*itr)->m_isHit = true;
			ObjectManager::object1.m_deleteObject.push_back(itr);
			m_HP--;
			if (speedType != SpeedType::Bomb)
			{
				m_timer = 0;
			}
			//スコア加算
			//Score::score++;
			//FLOAT2 size = { 10.0f, 17.0f };
			//ParticleManager::scoreParitcle.AddScore((*itr)->m_position, size, size, 1, 60);

			if (m_HP <= 0)
			{
				m_type = Angel;
				isDelete = true;
			}

			if (speedType == BaseEnemy::SpeedType::Normal)
			{
				FLOAT2 start = { 40,40 };
				FLOAT2 end = { 0, 0 };
				ParticleManager::speedType1.ExprotionParticle((*itr)->m_position, start, end, 10, 40);
			}
			else if (speedType == BaseEnemy::SpeedType::Midl)
			{
				FLOAT2 start = { 40,40 };
				FLOAT2 end = { 0, 0 };
				ParticleManager::speedType2.ExprotionParticle((*itr)->m_position, start, end, 10, 40);
			}
			else if (speedType == BaseEnemy::SpeedType::Hi)
			{
				FLOAT2 start = { 40,40 };
				FLOAT2 end = { 0, 0 };
				ParticleManager::speedType3.ExprotionParticle((*itr)->m_position, start, end, 10, 40);
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
			/*Score::score++;
			FLOAT2 size = { 10.0f, 17.0f };
			ParticleManager::scoreParitcle.AddScore((*itr)->m_position, size, size, 1, 60);*/

			if (m_HP <= 0)
			{
				m_type = Angel;
				isDelete = true;
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

void EnemyManager::AddEnemy(BaseEnemy::SpeedType f_speedType)
{
	BaseEnemy* enemy = new BaseEnemy();
	int a = rand() % 3;

	enemy->Init(f_speedType);

	EnemyManager::enemys.push_back(&(*enemy));
}

void EnemyManager::Update()
{
	for (auto itr = enemys.begin(); itr != enemys.end(); ++itr)
	{
		(*itr)->Update();
		if ((*itr)->isDelete)
		{
			if ((*itr)->speedType == BaseEnemy::SpeedType::Bomb)
			{
				if ((*itr)->isBombErase)
				{
					deleteEnemys.push_back(itr);
				}
				else
				{
					//ボム増やす？
					Player::AddBomb();
					deleteEnemys.push_back(itr);
				}
			}
			else
			{
				int score;
				if ((*itr)->speedType == BaseEnemy::SpeedType::Normal)
				{
					score = 10;
					/*FLOAT2 start = {40,40};
					FLOAT2 end = {0, 0};
					ParticleManager::speedType1.ExprotionParticle((*itr)->m_position, start, end, 10, 40);*/
				}
				else if ((*itr)->speedType == BaseEnemy::SpeedType::Midl)
				{
					score = 20;
					/*FLOAT2 start = {40,40 };
					FLOAT2 end = { 0, 0 };
					ParticleManager::speedType2.ExprotionParticle((*itr)->m_position, start, end, 10, 40);*/
				}
				else if ((*itr)->speedType == BaseEnemy::SpeedType::Hi)
				{
					score = 30;
					/*FLOAT2 start = { 40,40 };
					FLOAT2 end = { 0, 0 };
					ParticleManager::speedType3.ExprotionParticle((*itr)->m_position, start, end, 10, 40);*/
				}
				Score::score += score;
				FLOAT2 size = { 10.0f, 17.0f };
				ParticleManager::scoreParitcle.AddScore((*itr)->m_position, size, size, score, 60);
				deleteEnemys.push_back(itr);
			}
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
	DrawCircleAA(WindowSize::Wid / 2 + Shake::GetShake().u, WindowSize::Hi / 2 + Shake::GetShake().v, nowCenterR, 128, GetColor(200, 13, 13), 0, 1.0f);
	DrawCircleAA(WindowSize::Wid / 2, WindowSize::Hi / 2, nowTowerR, 128, GetColor(13, 200, 13), 0, 2.0f);
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
