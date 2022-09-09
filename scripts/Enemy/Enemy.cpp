#include "Enemy.h"

void BaseEnemy::Init(FLOAT2 f_position, FLOAT2 f_speed, int f_timer, float f_centerR)
{
	m_position = f_position;
	m_speed = f_speed;
	m_timer = f_timer;
	centerR = f_centerR;

}
