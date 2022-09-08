#pragma once
#include "DxLib.h"
#include <list>

struct UISprite
{
	FLOAT2 m_position;
	FLOAT2 m_startPos;
	FLOAT2 m_endPos;
	FLOAT2 m_size;
	bool m_isStart;//�o�Ă����Ƃ��ɃC�[�W���O���Ă���
	bool m_isMove;//�e���������
	bool m_isShot;//�����ꂽ��
	const float easeSpeed = 0.5f;
	int m_num;//���Ԗڂ���F��
	void EaseMove();
};

class BulletUI
{
	std::list<UISprite *> m_bullets;
	std::list<std::list<UISprite *>::iterator> m_deleteBullets;
	int m_sprite;
	//true�,false����
	bool m_isOddnumber;
	const int Xposition = 300;
	const int bulletDistance = 20;
	int standardPositionY;
	public:
	void AddBullet();
	void Init();
	void Update(int bulletNum);
	void Draw();
	void LoadFile();
	void LoadBullet();
	void ShotBullet();
};

class UIManager
{
	
};
