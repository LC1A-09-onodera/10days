#include "BaseObject.h"
#include "../Lib/Lib.h"
#include "../Math/Math.h"
#include "../Particle/Particle.h"
#include "../Scroll/Scroll.h"
#include "../Sound/Sound.h"
#include "DxLib.h"

#include <cmath>

ObjectSample ObjectManager::smp;
ObjectSample ObjectManager::object1;
ObjectSample ObjectManager::object2;
std::list<InducedExplosion*> ObjectManager::exprotionObject;
std::list<std::list<InducedExplosion*>::iterator> ObjectManager::deleteExprotionObject;
int InducedExplosion::m_s_exprosion[1];

bool BaseObject::IsMove = false;

float BaseObject::CiycleSpeed = 1;
bool BaseObject::isAllHit = false;
void ObjectSample::LoadFile(const char* path)
{
	m_sprite = LoadGraph(path);
}

void ObjectSample::Update(FLOAT2& f_playerPos, bool f_playerIsOutside)
{
	//全部の当たり判定を取る
	int index = 0;
	BaseObject::isAllHit = false;
	for (auto itr = m_objects.begin(); itr != m_objects.end(); ++itr)
	{
		//itr→メインのチェック対象
		//itr2→リストを回す
		auto itr2 = m_objects.begin();
		//今までチャックしたオブジェクトを飛ばすため
		for (int i = 0; i < index; i++)
		{
			itr2++;
		}
		for (; itr2 != m_objects.end(); ++itr2)
		{
			if (itr != itr2)
			{
				if ((*itr)->m_isShotMove && (*itr2)->m_isShotMove)
				{
					(*itr)->Collition(*(*itr2));
				}
			}
		}

		if (!f_playerIsOutside && (*itr)->m_isShotMove)
		{
			(*itr)->Collition(f_playerPos);
		}

		index++;
	}
	for (auto itr = m_objects.begin(); itr != m_objects.end(); ++itr)
	{
		(*itr)->Update();

		if ((*itr)->m_isHit)
		{
			m_deleteObject.push_back(itr);
		}
	}
	for (auto itr = m_deleteObject.begin(); itr != m_deleteObject.end(); ++itr)
	{
		m_objects.erase(*itr);
	}
	m_deleteObject.clear();
}

void ObjectSample::Shot(FLOAT2 f_position, FLOAT2 m_sprite, FLOAT2 f_direction, float R, BaseObject::ObjectType f_type)
{
	BaseObject* obj = new BaseObject();
	obj->Init(f_position, m_sprite, R, f_type);
	obj->Shot(f_direction);
	m_objects.push_back(obj);
}

void ObjectSample::Shot(FLOAT2 f_position, FLOAT2 m_sprite, float f_direction, float R, BaseObject::ObjectType f_type)
{
	BaseObject* obj = new BaseObject();
	obj->Init(f_position, m_sprite, R, f_type);
	obj->Shot(f_direction);
	m_objects.push_back(obj);
}

void ObjectSample::Draw()
{
	for (auto itr = m_objects.begin(); itr != m_objects.end(); ++itr)
	{
		//DrawGraph((*itr)->m_position.u, (*itr)->m_position.v, m_sprite, true);
		DrawExtendGraph((*itr)->m_position.u - ((*itr)->m_spriteSize.u / 2.0f), (*itr)->m_position.v - ((*itr)->m_spriteSize.v / 2.0f),
			(*itr)->m_position.u + ((*itr)->m_spriteSize.u / 2.0f), (*itr)->m_position.v + ((*itr)->m_spriteSize.v / 2.0f), m_sprite, true);
	}
}

void ObjectSample::Clear()
{
	m_objects.clear();
	m_deleteObject.clear();
}

BaseObject::BaseObject()
{
}

BaseObject::~BaseObject()
{
}

void BaseObject::Collition(BaseObject& object)
{
	if (Collision::CiycleCollision(this->m_position, this->m_R, object.m_position, object.m_R))
	{
		FLOAT2 l_shakePower = { 1.0f,1.0f };
		Shake::AddShakePower(l_shakePower);

		this->m_isHit = true;
		object.m_isHit = true;

		isAllHit = true;

		FLOAT2 startSize = { 30.0f, 30.0f };
		FLOAT2 endSize = { 0.0f, 0.0f };
		if (m_objectType == ObjectType::PINK)
		{
			ParticleManager::pinkParticle.ExprotionParticle(this->m_position, startSize, endSize, 6, 30);
		}
		else if (m_objectType == ObjectType::ORANGE)
		{
			ParticleManager::orangeParticle.ExprotionParticle(this->m_position, startSize, endSize, 6, 30);
		}
		else
		{
			ParticleManager::smpParticle.ExprotionParticle(this->m_position, startSize, endSize, 6, 30);
		}

		InducedExplosion* ind = new InducedExplosion();
		ind->Init(this->m_position, this->m_R + 30);
		ObjectManager::exprotionObject.push_back(ind);

		StopSoundMem(SoundManager::shotHitSound);
		PlaySoundMem(SoundManager::shotHitSound, DX_PLAYTYPE_BACK, true);
	}
}

void BaseObject::SetIsMove(bool move)
{
	IsMove = move;
}

bool BaseObject::GetIsAllHit()
{
	return isAllHit;
}

void BaseObject::Collition(FLOAT2& f_playerPos)
{
	if (Collision::CiycleCollision(this->m_position, this->m_R, f_playerPos, 20.0f))
	{
		this->m_isHit = true;
		FLOAT2 startSize = { 30.0f, 30.0f };
		FLOAT2 endSize = { 0.0f, 0.0f };
		//isAllHit = true;
		if (m_objectType == ObjectType::PINK)
		{
			ParticleManager::pinkParticle.ExprotionParticle(this->m_position, startSize, endSize, 6, 30);
		}
		else if (m_objectType == ObjectType::ORANGE)
		{
			ParticleManager::orangeParticle.ExprotionParticle(this->m_position, startSize, endSize, 6, 30);
		}
		else
		{
			ParticleManager::smpParticle.ExprotionParticle(this->m_position, startSize, endSize, 6, 30);
		}
		StopSoundMem(SoundManager::shotHitSound);
		PlaySoundMem(SoundManager::shotHitSound, DX_PLAYTYPE_BACK, true);
	}
}

void BaseObject::Shot(FLOAT2& direction)
{
	angleSub = direction;
}

void BaseObject::Shot(float direction)
{
	angle = direction;
}

void BaseObject::Update()
{
	//爆発する
	if (m_isHit)
	{

		return;
	}
	//円周に当たっているとき
	if (m_isShotMove && !BaseObject::IsMove)
	{
		angle += CiycleSpeed;
		float R = BaseObject::InsideR;
		m_position.u = centerPos.u + R * DxLibMath::Cos(angle);
		m_position.v = centerPos.v + R * DxLibMath::Sin(angle);
		FLOAT2 startSize = { 10.0f, 10.0f };
		FLOAT2 endSize = { 1.0f, 1.0f };
		ParticleManager::smpParticle.StayParticle(m_position, startSize, endSize, 1, 60);
		return;
	}

	//定数で動いていく
	float R = BaseObject::InsideR;
	m_position.u += R * DxLibMath::Cos(angle) * MoveSpeed;
	m_position.v += R * DxLibMath::Sin(angle) * MoveSpeed;
	m_nowR = Collision::Lenght(centerPos, m_position);
	if (rand() % 3 == 0)
	{
		FLOAT2 startSize = { 10.0f, 10.0f };
		FLOAT2 endSize = { 1.0f, 1.0f };
		ParticleManager::smpParticle.StayParticle(m_position, startSize, endSize, 4, 60);
	}
	if (m_nowR >= BaseObject::InsideR)
	{
		float angleSmp;
		m_nowR = BaseObject::InsideR;
		FLOAT2 vec;
		vec.u = m_position.u - centerPos.u;
		vec.v = m_position.v - centerPos.v;
		vec = Collision::Normalize(vec);
		m_position.u = centerPos.u + R * vec.u;
		m_position.v = centerPos.v + R * vec.v;
		angle = std::atan2(vec.v, vec.u) * 180.0f / 3.141592f;
		m_isShotMove = true;
	}
}

void BaseObject::Init(FLOAT2 position, FLOAT2 spriteSize, float R, ObjectType f_type)
{
	m_position = position;
	m_spriteSize = spriteSize;
	m_R = R;
	m_objectType = f_type;
}

void BaseObject::SpeedUpdate()
{
	CiycleSpeed += (float)IncreaseSpeed / 5.0f;
}

void BaseObject::ResetSpeed()
{
	CiycleSpeed = 1;
}

void BaseObject::ResetSpeed(int speed)
{
	CiycleSpeed = speed;
}

void ObjectManager::LoadFile()
{
	smp.LoadFile("Resources/particle.png");
	object1.LoadFile("Resources/object1.png");
	object2.LoadFile("Resources/object2.png");
}

void ObjectManager::Update(FLOAT2& f_playerPos, bool f_playerIsOutside)
{
	smp.Update(f_playerPos, f_playerIsOutside);
	object1.Update(f_playerPos, f_playerIsOutside);
	object2.Update(f_playerPos, f_playerIsOutside);
	AllCollision();

	if (Shake::GetPower().u > 0.0f)
	{
		FLOAT2 l_shakePower = { -0.1f,-0.1f };
		Shake::AddShakePower(l_shakePower);
	}
	if (Shake::GetPower().u < 0.0f)
	{
		FLOAT2 l_shakePower = Shake::GetPower();
		l_shakePower.u *= -1.0f;
		l_shakePower.v *= -1.0f;
		Shake::AddShakePower(l_shakePower);
	}
}

void ObjectManager::Draw()
{
	smp.Draw();
	object1.Draw();
	object2.Draw();
	for (auto itr = exprotionObject.begin(); itr != exprotionObject.end(); ++itr)
	{
		(*itr)->Draw();
	}
}

void ObjectManager::AllCollision()
{
	for (auto itr = object1.m_objects.begin(); itr != object1.m_objects.end(); ++itr)
	{
		for (auto itr2 = object2.m_objects.begin(); itr2 != object2.m_objects.end(); ++itr2)
		{
			(*itr)->Collition(*(*itr2));
		}
	}

	for (auto itr = exprotionObject.begin(); itr != exprotionObject.end(); ++itr)
	{
		for (auto itr2 = object1.m_objects.begin(); itr2 != object1.m_objects.end(); ++itr2)
		{
			(*itr)->Collition(*(*itr2));
		}
		for (auto itr3 = object2.m_objects.begin(); itr3 != object2.m_objects.end(); ++itr3)
		{
			(*itr)->Collition(*(*itr3));
		}
		(*itr)->Update();
		if ((*itr)->m_life <= 0)
		{
			deleteExprotionObject.push_back(itr);
		}
	}
	for (auto itr = deleteExprotionObject.begin(); itr != deleteExprotionObject.end(); ++itr)
	{
		exprotionObject.erase(*itr);
	}
	deleteExprotionObject.clear();
}

void ObjectManager::AllClear()
{
	smp.Clear();
	object1.Clear();
	object2.Clear();
	exprotionObject.clear();
	deleteExprotionObject.clear();
}

void InducedExplosion::LoadFile()
{
	m_s_exprosion[0] = LoadGraph("Resources/.png");
}

void InducedExplosion::Init(FLOAT2 f_position, float ExprosionR)
{
	m_position = f_position;
	this->ExplosionR = ExprosionR;
}

void InducedExplosion::Update()
{
	m_life--;

}

void InducedExplosion::Draw()
{
	//DrawExtendGraph(0, 0, 10 , 10,  InducedExplosion::m_s_exprosion[0], true);
	DrawCircle(m_position.u, m_position.v, ExplosionR, GetColor(255, 0, 255), false);
}

void InducedExplosion::Collition(BaseObject& obj)
{
	if (obj.m_isHit || !obj.m_isShotMove)
	{
		return;
	}
	if (Collision::CiycleCollision(m_position, ExplosionR, obj.m_position, obj.m_R))
	{
		FLOAT2 l_shakePower = { 1.0f,1.0f };
		Shake::AddShakePower(l_shakePower);

		obj.m_isHit = true;

		FLOAT2 startSize = { 30.0f, 30.0f };
		FLOAT2 endSize = { 0.0f, 0.0f };
		if (obj.m_objectType == BaseObject::ObjectType::PINK)
		{
			ParticleManager::pinkParticle.ExprotionParticle(this->m_position, startSize, endSize, 6, 30);
		}
		else if (obj.m_objectType == BaseObject::ObjectType::ORANGE)
		{
			ParticleManager::orangeParticle.ExprotionParticle(this->m_position, startSize, endSize, 6, 30);
		}
		else
		{
			ParticleManager::smpParticle.ExprotionParticle(this->m_position, startSize, endSize, 6, 30);
		}

		InducedExplosion* ind = new InducedExplosion();
		ind->Init(obj.m_position, obj.m_R + 5);
		ObjectManager::exprotionObject.push_back(ind);
	}
}
