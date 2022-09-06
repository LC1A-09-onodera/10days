#include "Scroll.h"

FLOAT2 Scroll::m_scrollValue = { 0, 0 };
FLOAT2 Shake::m_shakeValue = {0, 0 };
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

void Shake::AddShakePower(FLOAT2 f_shakePower)
{
	m_shakeValue = f_shakePower;
}

void Shake::ShakePowerUpdate()
{
}

float Shake::GetShapeX()
{
	return m_shakeValue.u;
}

float Shake::GetShapeY()
{
	return m_shakeValue.v;
}
