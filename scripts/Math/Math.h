#pragma once
#include "DxLib.h"
#include <cstdlib>

namespace DxLibMath
{
	static float PI = 3.141592f;
	static const int Cos(int angle)
	{
		return static_cast<int>(cos(angle * PI / 180.0f));
	}
	static const int Sin(int angle)
	{
		return static_cast<int>(sin(angle * PI / 180.0f));
	}
	static const float Cos(float angle)
	{
		return static_cast<float>(cos(angle * PI / 180.0f));
	}
	static const float Sin(float angle)
	{
		return static_cast<float>(sin(angle * PI / 180.0f));
	}
}