#include "../Score/Score.h"
#include "DxLib.h"
#include "../WindowsSize/WindowSize.h"

int Score::score;
int TowerHP::HP = TowerHP::MaxHP;
int TowerHP::s_HP;
int Score::GetScore()
{
	return score;
}

void TowerHP::Draw()
{
	DrawCircleGauge(WindowSize::Wid / 2, WindowSize::Hi / 2, 100.0f * ((float)HP / (float)MaxHP), s_HP, 0.0, 0.5f);
}
