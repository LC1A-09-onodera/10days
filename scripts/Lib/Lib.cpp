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

float Easeing::OutBounce(float time)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (time < 1.0f / d1) {
		return n1 * time * time;
	}
	else if (time < 2.0f / d1) {
		return n1 * (time -= 1.5f / d1) * time + 0.75f;
	}
	else if (time < 2.5f / d1) {
		return n1 * (time -= 2.25f / d1) * time + 0.9375f;
	}
	else {
		return n1 * (time -= 2.625f / d1) * time + 0.984375f;
	}
}

float Easeing::EaseOutBounce(float startPos, float endPos, float time)
{
	float hoge = endPos - startPos;
	const float n1 = 7.5625;
	const float d1 = 2.75;
	float result;
	if (time < 1.0f / d1)
	{
		result = startPos + OutBounce(time) * hoge;
		return result;
	}
	else if (time < 2.0f / d1) 
	{
		result = startPos + OutBounce(time) * hoge;
		return result;
	}
	else if (time < 2.5f / d1) 
	{
		result = startPos + OutBounce(time) * hoge;
		return result;
	}
	else 
	{
		result = startPos + OutBounce(time) * hoge;
		return result;
	}
}

FLOAT2 Easeing::EaseOutBounce(FLOAT2 startPos, FLOAT2 endPos, float time)
{
	FLOAT2 result;
	result.u = Easeing::EaseOutBounce(startPos.u, endPos.u, time);
	result.v = Easeing::EaseOutBounce(startPos.v, endPos.v, time);
	return result;
}

float Easeing::EaseInOutBounce(float startPos, float endPos, float time)
{
	float sub = endPos - startPos;
	if (time < 0.5f)
	{
		return startPos + (1 - OutBounce(1 - 2 * time)) / 2 * sub;
	}
	else
	{
		return startPos + (1 + OutBounce(2 * time - 1)) / 2 * sub;
	}
}

FLOAT2 Easeing::EaseInOutBounce(FLOAT2 startPos, FLOAT2 endPos, float time)
{
	FLOAT2 result;
	result.u = Easeing::EaseInOutBounce(startPos.u, endPos.u, time);
	result.v = Easeing::EaseInOutBounce(startPos.v, endPos.v, time);
	return result;
}

float Easeing::EaseOutElastic(float startPos, float endPos, float time)
{
	//const float c4 = (2.0f * 3.1415f) / 3.0f;

	//if (time)
	//return time == = 0 ? 0 : time == = 1 ? 1 : pow(2, -10 * time) * sin((time * 10 - 0.75) * c4) + 1;
	return float();
}
