#pragma once
class Score
{
	public:
	static int score;
	static int GetScore();
};

class TowerHP
{
	public:
	static int HP;
	static const int MaxHP = 3;
	static int s_HP;
	static int s_hart[4];
	static float nowAngle;
	//static float startAngle;
	static float endAngle;
	static void Draw();
};