#include "BaseObject.h"
#include "../Lib/Lib.h"
#include "../Math/Math.h"
#include "../Particle/Particle.h"

ObjectSample ObjectManager::smp;

void ObjectSample::LoadFile(const char* path)
{
	m_sprite = LoadGraph(path);
}

void ObjectSample::Update()
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

void ObjectSample::Shot(FLOAT2 f_position, FLOAT2 m_sprite, FLOAT2 f_direction, float R)
{
	BaseObject* obj = new BaseObject();
	obj->Init(f_position, m_sprite, R);
	obj->Shot(f_direction);
	m_objects.push_back(obj);
}

void ObjectSample::Shot(FLOAT2 f_position, FLOAT2 m_sprite, float f_direction, float R)
{
	BaseObject* obj = new BaseObject();
	obj->Init(f_position, m_sprite, R);
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
		this->m_isHit= true;
		object.m_isHit = true;
		
		FLOAT2 startSize = { 30.0f, 30.0f };
		FLOAT2 endSize = {0.0f, 0.0f };
		ParticleManager::smpParticle.ExprotionParticle(this->m_position, startSize, endSize, 6, 30);
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
		return;
	}

	//定数で動いていく
	float R = BaseObject::InsideR;
	m_position.u += R * DxLibMath::Cos(angle) * MoveSpeed;
	m_position.v += R * DxLibMath::Sin(angle) * MoveSpeed;
	m_nowR = Collision::Lenght(centerPos, m_position);
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
		m_isShotMove = true;
	}
}

void BaseObject::Init(FLOAT2 position, FLOAT2 spriteSize, float R)
{
	m_position = position;
	m_spriteSize = spriteSize;
	m_R = R;
}

void ObjectManager::LoadFile()
{
	smp.LoadFile("Resources/particle.png");
}

void ObjectManager::Update()
{
	smp.Update();
}

void ObjectManager::Draw()
{
	smp.Draw();
}
