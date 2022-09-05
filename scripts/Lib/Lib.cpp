#include "Lib.h"
#include <cmath>

using namespace std;

float Collision::Lenght(const FLOAT2& p1, const FLOAT2& p2)
{
	FLOAT2 lenght = { p1.u - p2.u , p1.v - p2.v };
	lenght.u = static_cast<float>(pow(lenght.u, 2));
	lenght.v = static_cast<float>(pow(lenght.v, 2));
	return sqrtf(lenght.u + lenght.v);
}

bool Collision::SquareCollision(const FLOAT2& f_LTop1, const FLOAT2& f_WidHi1, const FLOAT2& f_LTop2, const FLOAT2& f_WidHi2)
{
	
	return false;
}

bool Collision::CiycleCollision(const FLOAT2& f_Centre1, const float R1, const FLOAT2& f_Centre2, const float R2)
{
	if (Lenght(f_Centre1, f_Centre2) < R1 + R2)
	{
		return true;
	}
	else
	{
		return false;
	}
}