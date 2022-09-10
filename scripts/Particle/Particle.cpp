#include "Particle.h"
#include <thread>
#include "../Scroll/Scroll.h"
#include "../Lib/Lib.h"

Particle ParticleManager::smpParticle;
Particle ParticleManager::pinkParticle;
Particle ParticleManager::orangeParticle;
ScoreParticle ParticleManager::scoreParitcle;
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

void Particle::Clear()
{
	m_particles.clear();
	m_deleteParticles.clear();
}

void ParticleManager::LoadFile()
{
	smpParticle.LoadFile("Resources/particle_b.png");
	pinkParticle.LoadFile("Resources/ParticlePink.png");
	orangeParticle.LoadFile("Resources/ParticleOrange.png");
	scoreParitcle.LoadFile();
}

void ParticleManager::Update()
{
	smpParticle.Update();
	pinkParticle.Update();
	orangeParticle.Update();
	scoreParitcle.Update();
}

void ParticleManager::Draw()
{
	smpParticle.Draw();
	pinkParticle.Draw();
	orangeParticle.Draw();
	scoreParitcle.Draw();
}

void ParticleManager::AllClear()
{
	smpParticle.Clear();
	pinkParticle.Clear();
	orangeParticle.Clear();
	scoreParitcle.Clear();
}

void ScoreParticle::AddParitcle(FLOAT2& f_pos, FLOAT2& f_vec, FLOAT2& f_acc, FLOAT2& f_startSize, FLOAT2& f_endSize, int f_life)
{
	ParticleEach* each = new ParticleEach();
	each->position = f_pos;
	each->startSize = f_startSize;
	each->size = each->startSize;
	each->endSize = f_endSize;
	each->life = f_life;
	each->sLife = each->life;
	m_particles.push_back(each);
}

void ScoreParticle::AddParitcle(FLOAT2& f_pos, FLOAT2& endPos, int spriteNumber, FLOAT2& f_startSize, int f_life)
{
	ParticleEach* each = new ParticleEach();
	each->position = f_pos;
	each->startSize = f_startSize;
	each->size = each->startSize;
	each->endSize = each->startSize;
	each->endPosition = endPos;
	each->life = f_life;
	each->sLife = each->life;
	each->number = spriteNumber;
	m_particles.push_back(each);
}

void ScoreParticle::Update()
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

void ScoreParticle::EachUpdate(ParticleEach* itr)
{
	itr->position = Easeing::EaseInQuad(itr->position, itr->endPosition, itr->easeTimer);
	if (itr->easeTimer >= 1.0f)
	{
		itr->position = Easeing::EaseInQuad(itr->position, itr->endPosition, 1.0f);
		itr->easeTimer = 0;
		itr->life = 0;
	}
	else
	{
		itr->easeTimer += 0.01f;
		itr->life = 1;
	}
}

void ScoreParticle::Draw()
{
	for (auto itr = m_particles.begin(); itr != m_particles.end(); ++itr)
	{
		DrawExtendGraph((*itr)->position.u - ((*itr)->size.u / 2), (*itr)->position.v - ((*itr)->size.v / 2),
			(*itr)->position.u + ((*itr)->size.u / 2), (*itr)->position.v + ((*itr)->size.v / 2), m_sprite[(*itr)->number], true);
		//DrawExtendGraph((*itr)->position.u - ((*itr)->size.u / 2) - Scroll::GetScrollX(), (*itr)->position.v - ((*itr)->size.v / 2), (*itr)->position.u + ((*itr)->size.u / 2) - Scroll::GetScrollX(), (*itr)->position.v + ((*itr)->size.v / 2), m_sprite, true);
	}
}

void ScoreParticle::LoadFile()
{
	LoadDivGraph("Resources/numbers.png", 10, 5, 2, 700 / 5, 346 / 2, m_sprite);
	m_sprite[10] = LoadGraph("Resources/puls.png");
}

void ScoreParticle::AddScore(FLOAT2& f_emitPos, FLOAT2& f_startSize, FLOAT2& f_endSize, int f_addScore, int f_life)
{
	FLOAT2 emitPos = { 0 };
	FLOAT2 end = {0, 0};
	float leng = 50.0f;
	int dis = 10;
	if (f_addScore < 10)
	{
		emitPos = { f_emitPos.u - (f_startSize.u / 2.0f), f_emitPos.v + leng };
		end = emitPos;
		end.v -= leng;
		AddParitcle(emitPos, end, 10, f_startSize, f_life);
		emitPos.u += f_startSize.u / 2.0f + dis;
		end.u += f_startSize.u * 2.0f + dis;
		AddParitcle(emitPos, end, f_addScore, f_startSize, f_life);
	}
	else if (f_addScore < 100)
	{
		emitPos = { f_emitPos.u - (f_startSize.u / 2 * 2) , f_emitPos.v + leng };
		end = emitPos;
		end.v -= leng;
		AddParitcle(emitPos, end, 10, f_startSize, f_life);
		emitPos.u += f_startSize.u / 2.0f + dis;
		end.u += f_startSize.u / 2.0f + dis;
		AddParitcle(emitPos, end, f_addScore / 10 % 10, f_startSize, f_life);
		emitPos.u += f_startSize.u / 2.0f + dis;
		end.u += f_startSize.u / 2.0f + dis;
		AddParitcle(emitPos, end, f_addScore % 10, f_startSize, f_life);
	}
	else if (f_addScore < 1000)
	{
		emitPos = { f_emitPos.u - (f_startSize.u / 2 * 3) , f_emitPos.v + leng };
		end = emitPos;
		end.v -= leng;
		AddParitcle(emitPos, end, 10, f_startSize, f_life);
		emitPos.u += f_startSize.u / 2.0f + dis;
		end.u += f_startSize.u / 2.0f + dis;
		AddParitcle(emitPos, end, f_addScore / 10 % 10, f_startSize, f_life);
		emitPos.u += f_startSize.u / 2.0f + dis;
		end.u += f_startSize.u / 2.0f + dis;
		AddParitcle(emitPos, end, f_addScore % 10, f_startSize, f_life);
		emitPos.u += f_startSize.u / 2.0f + dis;
		end.u += f_startSize.u / 2.0f + dis;
		AddParitcle(emitPos, end, f_addScore % 10, f_startSize, f_life);
	}
	else if (f_addScore < 10000)
	{

	}
}

void ScoreParticle::Clear()
{
	m_particles.clear();
	m_deleteParticles.clear();
}

