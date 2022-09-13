#pragma once
#include "DxLib.h"
#include <list>

struct UISprite
{
	FLOAT2 m_position;
	FLOAT2 m_startPos;
	FLOAT2 m_endPos;
	FLOAT2 m_size;
	bool m_isStart;//出てきたときにイージングしている
	bool m_isMove;//弾を消費し動く
	bool m_isShot;//撃たれた時
	const float easeSpeed = 0.5f;
	int m_num;//何番目かを認識
	float angle;
	void EaseMove();
};

class BulletUI
{
	
	std::list<std::list<UISprite *>::iterator> m_deleteBullets;
	int m_sprite;
	//true奇数,false偶数
	bool m_isOddnumber;
	const int Xposition = 300;
	const int bulletDistance = 20;
	int standardPositionY;
	float spornAngle = 0;

	const int SpornR = 1500;
	const int BulletR = 220;
	public:
	std::list<UISprite *> m_bullets;
	bool m_isAllShot = false;
	void AddBullet();
	void AddBullet2();
	void Init();
	void Update(int bulletNum);
	void Update1();
	void Update2();
	void Draw();
	void LoadFile();
	void LoadBullet();
	void ShotBullet();

	void AllShotStart();

	void AllShot();
	int BulletNum();
	bool GetIsAllShot();
};

class HPUI
{
	std::list<UISprite*> m_HPs;
	std::list<std::list<UISprite*>::iterator> m_deleteHPs;
	int m_sprite;
	void Init();
	void Update(int bulletNum);
	void Draw();
	void LoadFile();
};

class ScoreUI
{
public:
	static std::list<UISprite *> m_scores;
	static int s_numbers[10];
	static int nowScore;
	static float ext;
	static FLOAT2 scorePos[5];
	static int s_score[5];
	static const int R = 80;

	static void LoadFile();
	static void Init();
	static void Update(int score);
	static void Draw();
};

class UIManager
{
	
};
