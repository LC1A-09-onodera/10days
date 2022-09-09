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
	void EaseMove();
};

class BulletUI
{
	std::list<UISprite *> m_bullets;
	std::list<std::list<UISprite *>::iterator> m_deleteBullets;
	int m_sprite;
	//true奇数,false偶数
	bool m_isOddnumber;
	const int Xposition = 300;
	const int bulletDistance = 20;
	int standardPositionY;
	
	public:
	bool m_isAllShot = false;
	void AddBullet();
	void Init();
	void Update(int bulletNum);
	void Draw();
	void LoadFile();
	void LoadBullet();
	void ShotBullet();

	void AllShotStart();

	void AllShot();
	int BulletNum();
	bool GetIsAllShot();
};

class UIManager
{
	
};
