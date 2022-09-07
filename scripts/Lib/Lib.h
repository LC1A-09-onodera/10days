#pragma once
#include "DxLib.h"

namespace Collision
{
	float Lenght(const FLOAT2 &p1, const FLOAT2 &p2);
	float Lenght(const FLOAT2& p1);
	FLOAT2 Normalize(const FLOAT2 &p1);
	bool SquareCollision(const FLOAT2 &f_LTop1, const FLOAT2 &f_WidHi1, const FLOAT2 &f_LTop2, const FLOAT2 &f_WidHi2);
	bool CiycleCollision(const FLOAT2 &f_Centre1, const float R1, const FLOAT2& f_Centre2, const float R2);
}

namespace Easeing
{
	float EaseOutBounce();
}
