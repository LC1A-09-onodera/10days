#pragma once
#include "DxLib.h"
#include <list>

class Particle
{
	struct Emitter
	{
		FLOAT2 position = { 0, 0 };
		float life = 0;
	};
	struct ParticleEach
	{
		FLOAT2 position = {0, 0};
		FLOAT2 vector = { 0, 0 };
		FLOAT2 acceleration = { 0, 0 };
		int life = 60;
		int sLife = 60;
		FLOAT2 size = { 0, 0 };
		FLOAT2 startSize = { 0, 0 };
		FLOAT2 endSize = { 0, 0 };
	};

	std::list<ParticleEach *> m_particles;
	std::list<std::list<ParticleEach *>::iterator> m_deleteParticles;
	Emitter m_emitter;
	int m_sprite;
public:
	void AddParitcle(FLOAT2 &f_pos, FLOAT2 & f_vec, FLOAT2 & f_acc, FLOAT2 & f_startSize, FLOAT2 & f_endSize, int f_life);
public:
	void Start();
	void Update();
	void EachUpdate(ParticleEach *itr);
	void Draw();
	void LoadFile(const char *path);
	void FlyParticle(FLOAT2 &f_emitPos, FLOAT2& f_startSize, FLOAT2& f_endSize, int f_count, int f_life);
	void ExprotionParticle(FLOAT2& f_emitPos, FLOAT2& f_startSize, FLOAT2& f_endSize, int f_count, int f_life);
	void StayParticle(FLOAT2& f_emitPos, FLOAT2& f_startSize, FLOAT2& f_endSize, int f_count, int f_life);
};

class ParticleManager
{
public:
	static Particle smpParticle;
	static Particle pinkParticle;
	static Particle orangeParticle;
	static void LoadFile();
	static void Update();
	static void Draw();
};