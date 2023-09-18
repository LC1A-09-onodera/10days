#include "Scroll.h"
#include <cmath>

FLOAT2 Scroll::m_scrollValue = { 0, 0 };
FLOAT2 Shake::m_shakeValue = { 0, 0 };
FLOAT2 Shake::m_randPower = { 0,0 };
float Shake::m_maxPower = 30.0f;
bool Shake::m_isAddPowerTrigger = false;

void Scroll::AddSCroll(FLOAT2 f_addValue)
{
	m_scrollValue.u = m_scrollValue.u + f_addValue.u;
	m_scrollValue.v = m_scrollValue.v + f_addValue.v;
}

FLOAT2 Scroll::GetScroll()
{
	return m_scrollValue;
}

float Scroll::GetScrollX()
{
	return m_scrollValue.u;
}

float Scroll::GetScrollY()
{
	return m_scrollValue.v;
}

void Shake::SetShakePower(FLOAT2 f_shakePower)
{
	m_shakeValue = f_shakePower;
}

void Shake::AddShakePower(FLOAT2 f_shakePower)
{
	//ñàÉtÉåÅ[ÉÄïœÇ¶ÇÈÇ∆Ç®Ç©ÇµÇ≠Ç»ÇËÇªÇ§ÇæÇ©ÇÁ
	if (!m_isAddPowerTrigger)
	{
		if (m_shakeValue.u <= m_maxPower)
		{
			m_shakeValue.u += f_shakePower.u;
		}
		if (m_shakeValue.u > m_maxPower)
		{
			m_shakeValue.u = m_maxPower;
		}
		if (m_shakeValue.v <= m_maxPower)
		{
			m_shakeValue.v += f_shakePower.v;
		}
		if (m_shakeValue.v > m_maxPower)
		{
			m_shakeValue.v = m_maxPower;
		}

		if (m_shakeValue.u < 0.0f)
		{
			m_shakeValue.u = 0.0f;
		}
		if (m_shakeValue.v < 0.0f)
		{
			m_shakeValue.v = 0.0f;
		}
	}
}

FLOAT2 Shake::GetShake()
{
	if (!m_isAddPowerTrigger)
	{
		FLOAT2 l_power = { 0,0 };
		float l_randRad = rand() % 315 / 100.0f;
		m_randPower.u = sinf(l_randRad) * m_shakeValue.u;
		m_randPower.v = cosf(l_randRad) * m_shakeValue.v;
		l_power.u = m_randPower.u;
		l_power.v = m_randPower.v;
		m_isAddPowerTrigger = true;
		return l_power;
	}
	else
	{
		FLOAT2 l_power = { 0,0 };
		l_power.u = -m_randPower.u;
		l_power.v = -m_randPower.v;
		m_isAddPowerTrigger = false;
		return l_power;
	}
}

FLOAT2 Shake::GetPower()
{
	return m_shakeValue;
}

void Shake::ShakePowerUpdate()
{
}

float Shake::GetPowerX()
{
	return m_shakeValue.u;
}

float Shake::GetPowerY()
{
	return m_shakeValue.v;
}
