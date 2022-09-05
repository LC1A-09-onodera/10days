#pragma once
#include "DxLib.h"

class Player
{
private:
	//スプライト
	int m_sprite;
	FLOAT2 m_position = { 0.0f, 0.0f };
	//絵の向き
	FLOAT2 m_rotation = { 0.0f, 0.0f };
	//加算処理時に加えられる力
	FLOAT2 m_addForce = { 0.0f, 0.0f };
	//減算用power
	FLOAT2 m_subForce = { 0.0f, 0.0f };
	//ジェットパックの向き
	FLOAT2 m_jettRotation = { 0.0f, 0.0f };
	//加算重力
	FLOAT2 m_gravity = { 0.0f, 0.0f };
	//向き
	FLOAT2 m_direction = { 0.0f, 0.0f };
public:
	void Init();
	void Update();
	void Draw();
	void LoadFile();

private:
	void AddForce();
	void AttachForce();
	
};
