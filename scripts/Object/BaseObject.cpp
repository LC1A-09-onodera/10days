#include "BaseObject.h"
#include "../Lib/Lib.h"
#include "../Math/Math.h"
#include "../Particle/Particle.h"
#include <cmath>

ObjectSample ObjectManager::smp;
ObjectSample ObjectManager::object1;
ObjectSample ObjectManager::object2;
void ObjectSample::LoadFile(const char* path)
{
	m_sprite = LoadGraph(path);
}

void ObjectSample::Update(FLOAT2& f_playerPos, bool f_playerIsOutside)
{
	//全部の当たり判定を取る
	int index = 0;

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
				(*itr)->Collition(*(*itr2));
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
		this->m_isHit = true;
		object.m_isHit = true;

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
	}
}

void BaseObject::Collition(FLOAT2& f_playerPos)
{
	if (Collision::CiycleCollision(this->m_position, this->m_R, f_playerPos, 20.0f))
	{
		this->m_isHit = true;
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
	if (m_isShotMove)
	{
		angle += CiycleSpeed;
		float R = BaseObject::InsideR;
		m_position.u = centerPos.u + R * DxLibMath::Cos(angle);
		m_position.v = centerPos.v + R * DxLibMath::Sin(angle);
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
}

void ObjectManager::Draw()
{
	smp.Draw();
	object1.Draw();
	object2.Draw();
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
}
