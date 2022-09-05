#include "Particle.h"
#include <thread>

void Particle::AddParitcle(FLOAT2& f_pos, FLOAT2& f_vec, FLOAT2& f_acc, FLOAT2& f_startSize, FLOAT2& f_endSize, int f_life)
{
	ParticleEach *each = new ParticleEach();
	each->position = f_pos;
	each->vector = f_vec;
	each->acceleration = f_acc;
	each->startSize = f_startSize;
	each->size = each->startSize;
	each->endSize = f_endSize;
	each->life = f_life;
	each->sLife = each->life;
	m_particles.push_back(each);
}

void Particle::Update()
{
	for (auto itr = m_particles.begin(); itr != m_particles.end(); ++itr)
	{
		if ((*itr)->life <= 0)
		{
			m_deleteParticles.push_back(itr);
			continue;
		}
		EachUpdate((*itr));
	}
	for (auto itr = m_deleteParticles.begin(); itr != m_deleteParticles.end(); ++itr)
	{
		m_particles.erase((*itr));
	}
	m_deleteParticles.clear();
}

void Particle::EachUpdate(ParticleEach *itr)
{
	itr->life--;
	itr->position.u += itr->vector.u;
	itr->position.v += itr->vector.v;
	itr->vector.u += itr->acceleration.u;
	itr->vector.v += itr->acceleration.v;
	float u = (itr->endSize.u - itr->startSize.u) / itr->sLife;
	float v = (itr->endSize.v - itr->startSize.v) / itr->sLife;

	itr->size.u +=  u;
	itr->size.v +=  v;
}

void Particle::Draw()
{
	for (auto itr = m_particles.begin(); itr != m_particles.end(); ++itr)
	{
		DrawExtendGraph((*itr)->position.u - ((*itr)->size.u / 2), (*itr)->position.v - ((*itr)->size.v / 2), (*itr)->position.u + ((*itr)->size.u / 2), (*itr)->position.v + ((*itr)->size.v / 2), m_sprite, true);
	}
}

void Particle::LoadFile(const char* path)
{
	m_sprite = LoadGraph(path);
}