#include "Particle.h"
#include <thread>
#include "../Scroll/Scroll.h"


Particle ParticleManager::smpParticle;
Particle ParticleManager::pinkParticle;
Particle ParticleManager::orangeParticle;
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
		DrawExtendGraph((*itr)->position.u - ((*itr)->size.u / 2), (*itr)->position.v - ((*itr)->size.v / 2), 
						(*itr)->position.u + ((*itr)->size.u / 2), (*itr)->position.v + ((*itr)->size.v / 2), m_sprite, true);
		//DrawExtendGraph((*itr)->position.u - ((*itr)->size.u / 2) - Scroll::GetScrollX(), (*itr)->position.v - ((*itr)->size.v / 2), (*itr)->position.u + ((*itr)->size.u / 2) - Scroll::GetScrollX(), (*itr)->position.v + ((*itr)->size.v / 2), m_sprite, true);
	}
}

void Particle::LoadFile(const char* path)
{
	m_sprite = LoadGraph(path);
}

void Particle::FlyParticle(FLOAT2 &f_emitPos, FLOAT2& f_startSize, FLOAT2& f_endSize, int f_count, int f_life)
{
	for (int i = 0; i < f_count; i++)
	{
		FLOAT2 vel = { 0 };
		vel.v = rand() % 21 - 10;
		FLOAT2 acc = { 0 };
		acc.v = vel.v / f_life;
		AddParitcle(f_emitPos, vel, acc, f_startSize, f_endSize, f_life);
	}
}

void Particle::ExprotionParticle(FLOAT2& f_emitPos, FLOAT2& f_startSize, FLOAT2& f_endSize, int f_count, int f_life)
{
	for (int i = 0; i < f_count; i++)
	{
		FLOAT2 vel = { 0 };
		vel.u = rand() % 21 - 10;
		vel.v = rand() % 21 - 10;
		FLOAT2 acc = { 0 };
		acc.u = -vel.u / f_life;
		acc.v = -vel.v / f_life;
		AddParitcle(f_emitPos, vel, acc, f_startSize, f_endSize, f_life);
	}
}

void Particle::StayParticle(FLOAT2& f_emitPos, FLOAT2& f_startSize, FLOAT2& f_endSize, int f_count, int f_life)
{
	for (int i = 0; i < f_count; i++)
	{
		FLOAT2 vel = { 0 };
		vel.u = rand() % 3 - 1;
		vel.v = rand() % 3 - 1;
		FLOAT2 acc = { 0 };
		acc.u = -vel.u / f_life;
		acc.v = -vel.v / f_life;
		AddParitcle(f_emitPos, vel, acc, f_startSize, f_endSize, f_life);
	}
}

void Particle::SampleParticle(FLOAT2& f_startSize, FLOAT2& f_endSize, int f_count, int f_life)
{
	for (int i = 0; i < f_count; i++)
	{
		FLOAT2 emitPos = { 0 };
		FLOAT2 vel = { 0 };
		vel.u = rand() % 3 - 1;
		vel.v = rand() % 3 - 1;
		FLOAT2 acc = { 0 };
		acc.u = -vel.u / f_life;
		acc.v = -vel.v / f_life;
		AddParitcle(emitPos, vel, acc, f_startSize, f_endSize, f_life);
	}
}

void ParticleManager::LoadFile()
{
	smpParticle.LoadFile("Resources/particle.png");
	pinkParticle.LoadFile("Resources/ParticlePink.png");
	orangeParticle.LoadFile("Resources/ParticleOrange.png");
}

void ParticleManager::Update()
{
	smpParticle.Update();
	pinkParticle.Update();
	orangeParticle.Update();
}

void ParticleManager::Draw()
{
	smpParticle.Draw();
	pinkParticle.Draw();
	orangeParticle.Draw();
}
