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
	static FLOAT2 m_randPower;
	static float m_maxPower;
	static bool m_isAddPowerTrigger;
public:
	static void AddShakePower(FLOAT2 f_shakePower);
	static FLOAT2 GetShake();
	static FLOAT2 GetPower();
	static void ShakePowerUpdate();
	static float GetPowerX();
	static float GetPowerY();
};