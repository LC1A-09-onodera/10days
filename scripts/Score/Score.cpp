#include "../Score/Score.h"
#include "DxLib.h"
#include "../WindowsSize/WindowSize.h"
#include "../Lib/Lib.h"

int Score::score;
int TowerHP::HP = TowerHP::MaxHP;
int TowerHP::s_HP;

float TowerHP::nowAngle = 0;
float TowerHP::endAngle = 100;
int TowerHP::s_hart[4];
int Score::GetScore()
{
	return score;
}

void TowerHP::Draw()
{
	endAngle = 100.0f * ((float)HP / (float)MaxHP);
	nowAngle = Easeing::EaseInQuad(nowAngle, endAngle, 0.25f);
	float sen = 0.3f;
	FLOAT2 size = {344 * sen, 315 * sen};
	DrawExtendGraph(WindowSize::Wid / 2 - size.u / 2, WindowSize::Hi / 2 - size.v / 2, WindowSize::Wid / 2 + size.u / 2, WindowSize::Hi / 2 + size.v / 2,
					s_hart[HP], true);
	//DrawCircleGauge(WindowSize::Wid / 2, WindowSize::Hi / 2, nowAngle, s_HP, 0.0, 0.3f);
}
