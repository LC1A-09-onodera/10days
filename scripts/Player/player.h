#pragma once
#include "DxLib.h"

class Player
{
private:
	int m_sprite;
	FLOAT2 m_position;
	FLOAT2 m_rotation;
	FLOAT2 m_addForce;
	FLOAT2 m_subForce;
	FLOAT2 m_jettRotation;
	FLOAT2 m_gravity;
public:
	void Init();
	void Update();
	void Draw();
	void LoadFile();

private:
	void AddForce();
	void AttachForce();
	
};
