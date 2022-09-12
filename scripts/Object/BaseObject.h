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

	enum class ObjectType
	{
		ORANGE,
		PINK
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

	ObjectType m_objectType;

	//打ち出した時の方向どっちで行こうか
	float angle = 0.0f;
	FLOAT2 angleSub = {0 , 0};

	static const int InsideR = 222;
	static const int OutshideR = 320;
	const float MoveSpeed = 0.09f;
	static float CiycleSpeed;
	static const int IncreaseSpeed = 1;

	static bool isAllHit;
	static bool GetIsAllHit();

	static bool IsMove;
	static void SetIsMove(bool move);

	FLOAT2 centerPos = { 1280 / 2 , 720 / 2 };

	void Collition(BaseObject& object);
	void Collition(FLOAT2& f_playerPos);
	void Shot(FLOAT2& direction);
	void Shot(float direction);
	void Update();
	void Init(FLOAT2 position, FLOAT2 spriteSize, float R, ObjectType f_type);
	static void SpeedUpdate();
	static void ResetSpeed();
	static void ResetSpeed(int speed);
};

struct InducedExplosion
{
	float ExplosionR;
	static int m_s_exprosion[1];
	FLOAT2 m_position;
	int m_life = 2;
	static void LoadFile();
	void Init(FLOAT2 f_position, float ExprosionR);
	void Update();
	void Draw();
	void Collition(BaseObject &obj);
};

class ObjectSample
{
	int m_sprite;
public:
	std::list<BaseObject*> m_objects;
	std::list<std::list<BaseObject*>::iterator> m_deleteObject;
	void LoadFile(const char* path);
	void Update(FLOAT2& f_playerPos, bool f_playerIsOutside);
	void Shot(FLOAT2 f_position, FLOAT2 m_sprite, FLOAT2 f_direction, float R, BaseObject::ObjectType f_type);
	void Shot(FLOAT2 f_position, FLOAT2 m_sprite, float f_direction, float R, BaseObject::ObjectType f_type);
	void Draw();
	void Clear();
};

class ObjectManager
{
public:
	static ObjectSample smp;
	static ObjectSample object1;
	static ObjectSample object2;
	static std::list<InducedExplosion *> exprotionObject;
	static std::list<std::list<InducedExplosion*>::iterator> deleteExprotionObject;
	static void LoadFile();
	static void Update(FLOAT2 &f_playerPos, bool f_playerIsOutside);
	static void Draw();
	static void AllCollision();
	static void AllClear();
};



