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

float Collision::Lenght(const FLOAT2& p1)
{
	FLOAT2 lenght = p1;
	lenght.u = static_cast<float>(pow(lenght.u, 2));
	lenght.v = static_cast<float>(pow(lenght.v, 2));
	return sqrtf(lenght.u + lenght.v);
}

FLOAT2 Collision::Normalize(const FLOAT2& p1)
{
	float leng = Lenght(p1);
	FLOAT2 result;
	result.u = p1.u / leng;
	result.v = p1.v / leng;
	return result;
}

bool Collision::SquareCollision(const FLOAT2& f_LTop1, const FLOAT2& f_WidHi1, const FLOAT2& f_LTop2, const FLOAT2& f_WidHi2)
{
	// xŽ²‚É‘Î‚µ‚Ä‚Ì“–‚½‚è”»’è
	if (f_LTop1.u + f_WidHi1.u >= f_LTop2.u && f_LTop1.u <= f_LTop2.u + f_WidHi2.u)
	{
		// yŽ²
		if (f_LTop1.v + f_WidHi1.v >= f_LTop2.v && f_LTop1.v <= f_LTop2.v + f_WidHi2.v)
		{
			return true;
		}
	}
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