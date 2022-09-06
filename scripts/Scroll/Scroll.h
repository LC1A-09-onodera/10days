#pragma once
#include "DxLib.h"

class Scroll
{
private:
	static FLOAT2 m_scrollValue;
public:
	static void AddSCroll(FLOAT2 f_addValue);
	static FLOAT2 GetScroll();
	static float GetScrollX();
	static float GetScrollY();
};

class Shake
{
private:
	static FLOAT2 m_shakeValue;
public:
	static void AddShakePower(FLOAT2 f_shakePower);
	static void ShakePowerUpdate();
	static float GetShapeX();
	static float GetShapeY();
};