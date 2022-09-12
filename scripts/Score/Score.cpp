#include "../Score/Score.h"
#include "DxLib.h"
#include "../WindowsSize/WindowSize.h"
#include "../Lib/Lib.h"

int Score::score;
int TowerHP::HP = TowerHP::MaxHP;
int TowerHP::s_HP;

float TowerHP::nowAngle = 100;
float TowerHP::endAngle = 100;

int Score::GetScore()
{
	return score;
}

void TowerHP::Draw()
{
	endAngle = 100.0f * ((float)HP / (float)MaxHP);
	nowAngle = Easeing::EaseInQuad(nowAngle, endAngle, 0.2f);
	DrawCircleGauge(WindowSize::Wid / 2, WindowSize::Hi / 2, nowAngle, s_HP, 0.0, 0.5f);
}
