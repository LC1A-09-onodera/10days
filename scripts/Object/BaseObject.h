#pragma once
#include "DxLib.h"
#include <list>

struct BaseObject
{
	BaseObject();
	~BaseObject();

	enum class Shide
	{
		INSHIDE,
		OUTSHIDE,
	};

	FLOAT2 m_position = { 0, 0 };
	FLOAT2 m_spriteSize = {100, 100};
	//当たり判定用半径
	float m_R = 1.0f;
	//動いている間
	bool m_isShotMove = false;
	//ほかのオブジェクトと当たった時
	bool m_isHit = false;
	
	float m_nowR = 0.0f;
	//相対位置の割合
	float m_easeR = 0.0f;

	Shide m_shide = Shide::INSHIDE;

	//打ち出した時の方向どっちで行こうか
	float angle = 0.0f;
	FLOAT2 angleSub = {0 , 0};

	static const int InsideR = 300;
	static const int OutshideR = 320;
	const float MoveSpeed = 0.05f;

	FLOAT2 centerPos = { 1280 / 2 , 720 / 2 };

	void Collition(BaseObject& object);
	void Shot(FLOAT2& direction);
	void Shot(float direction);
	void Update();
	void Init(FLOAT2 position, FLOAT2 spriteSize, float R);
};

class ObjectSample
{
	std::list<BaseObject*> m_objects;
	std::list<std::list<BaseObject *>::iterator> m_deleteObject;
	int m_sprite;
public:
	void LoadFile(const char* path);
	void Update();
	void Shot(FLOAT2 f_position, FLOAT2 m_sprite, FLOAT2 f_direction, float R);
	void Shot(FLOAT2 f_position, FLOAT2 m_sprite, float f_direction, float R);
	void Draw();
};

class ObjectManager
{
public:
	static ObjectSample smp;

	static void LoadFile();
	static void Update();
	static void Draw();
};



