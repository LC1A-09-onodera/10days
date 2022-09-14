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
		FLOAT2 position = { 0, 0 };
		FLOAT2 vector = { 0, 0 };
		FLOAT2 acceleration = { 0, 0 };
		int life = 60;
		int sLife = 60;
		FLOAT2 size = { 0, 0 };
		FLOAT2 startSize = { 0, 0 };
		FLOAT2 endSize = { 0, 0 };
	};

	std::list<ParticleEach*> m_particles;
	std::list<std::list<ParticleEach*>::iterator> m_deleteParticles;
	Emitter m_emitter;
	int m_sprite;
public:
	void AddParitcle(FLOAT2& f_pos, FLOAT2& f_vec, FLOAT2& f_acc, FLOAT2& f_startSize, FLOAT2& f_endSize, int f_life);
public:
	void Start();
	void Update();
	void EachUpdate(ParticleEach* itr);
	void Draw();
	void LoadFile(const char* path);
	void FlyParticle(FLOAT2& f_emitPos, FLOAT2& f_startSize, FLOAT2& f_endSize, int f_count, int f_life);
	void ExprotionParticle(FLOAT2& f_emitPos, FLOAT2& f_startSize, FLOAT2& f_endSize, int f_count, int f_life);
	void StayParticle(FLOAT2& f_emitPos, FLOAT2& f_startSize, FLOAT2& f_endSize, int f_count, int f_life);

	void AddScore(FLOAT2& f_emitPos, FLOAT2& f_startSize, FLOAT2& f_endSize, int f_addScore, int f_life);

	void SampleParticle(FLOAT2& f_startSize, FLOAT2& f_endSize, int f_count, int f_life);
	void Clear();
};

class ScoreParticle
{
	struct Emitter
	{
		FLOAT2 position = { 0, 0 };
		float life = 0;
	};
	struct ParticleEach
	{
		FLOAT2 position = { 0, 0 };
		FLOAT2 endPosition = { 0, 0 };
		int life = 60;
		int sLife = 60;
		FLOAT2 size = { 0, 0 };
		FLOAT2 startSize = { 0, 0 };
		FLOAT2 endSize = { 0, 0 };
		int number = -1;
		float easeTimer = 0.0f;
	};

	std::list<ParticleEach*> m_particles;
	std::list<std::list<ParticleEach*>::iterator> m_deleteParticles;
	Emitter m_emitter;
	int m_sprite[11];
	int m_num;
public:
	void AddParitcle(FLOAT2& f_pos, FLOAT2& f_vec, FLOAT2& f_acc, FLOAT2& f_startSize, FLOAT2& f_endSize, int f_life);
	void AddParitcle(FLOAT2& f_pos, FLOAT2 &endPos, int spriteNumber ,FLOAT2& f_startSize,  int f_life);
public:
	void Update();
	void EachUpdate(ParticleEach* itr);
	void Draw();
	void LoadFile();
	void AddScore(FLOAT2& f_emitPos, FLOAT2& f_startSize, FLOAT2& f_endSize, int f_addScore, int f_life);
	void Clear();
};

class ParticleManager
{
public:
	static Particle smpParticle;
	static Particle pinkParticle;
	static Particle orangeParticle;
	static ScoreParticle scoreParitcle;
	static Particle circleParticle;
	static Particle cubeParticle;
	static Particle speedType1;
	static Particle speedType2;
	static Particle speedType3;
	static void LoadFile();
	static void Update();
	static void Draw();
	static void AllClear();
};