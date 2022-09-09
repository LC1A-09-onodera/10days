#include "Scene.h"
#include "../Scroll/Scroll.h"
#include "../WindowsSize/WindowSize.h"

int TitleScene::m_s_side;
FLOAT2 TitleScene::m_sideSize;

int GameScene::m_s_side;
FLOAT2 GameScene::m_sideSize;


int TitleScene::m_s_title;
int TitleScene::m_s_space;
FLOAT2 TitleScene::m_titlePos;
FLOAT2 TitleScene::m_spacePos;
FLOAT2 TitleScene::m_titleSize;
FLOAT2 TitleScene::m_spaceSize;
int TitleScene::m_s_ciycle;
FLOAT2 TitleScene::m_ciycleSize = { 504, 504 };

FLOAT2 SceneSide::m_sidePosition[4];

int GameScene::sceneTimer = 300;

void GameScene::LoadFile()
{
	m_s_side = LoadGraph("Resources/EncanEncounter.png");
	m_sideSize = {1139.0f, 104.0f};
}

void GameScene::Update()
{
	float speed = 4.0f;
	for (int i = static_cast<int>(Direction::Right1); i <= static_cast<int>(Direction::Right2); i++)
	{
		SceneSide::m_sidePosition[i].v -= speed;
		if (SceneSide::m_sidePosition[i].v < -m_sideSize.u / 2)
		{
			SceneSide::m_sidePosition[i].v = m_sideSize.u + (m_sideSize.u / 2);
		}
	}
	for (int i = static_cast<int>(Direction::Left1); i <= static_cast<int>(Direction::Left2); i++)
	{
		SceneSide::m_sidePosition[i].v += speed;
		if (SceneSide::m_sidePosition[i].v > WindowSize::Hi + m_sideSize.u / 2)
		{
			SceneSide::m_sidePosition[i].v = -((m_sideSize.u - WindowSize::Hi) + m_sideSize.u / 2);
		}
	}
}

void GameScene::Init()
{
	for (int i = static_cast<int>(Direction::Right1); i <= static_cast<int>(Direction::Right2); i++)
	{
		SceneSide::m_sidePosition[i] = {0, WindowSize::Wid + (m_sideSize.u * i)};
	}
	for (int i = static_cast<int>(Direction::Left1); i <= static_cast<int>(Direction::Left2); i++)
	{
		SceneSide::m_sidePosition[i] = { WindowSize::Wid - m_sideSize.v, (-m_sideSize.u / 2) - (m_sideSize.u * (i - 2))};
	}
}

void GameScene::Draw()
{
	for (int i = static_cast<int>(Direction::Right1); i <= static_cast<int>(Direction::Right2); i++)
	{
		DrawRotaGraph(SceneSide::m_sidePosition[i].u + m_sideSize.v / 2.0f, SceneSide::m_sidePosition[i].v,
			1.0f, 3.1415f / 2.0f,
			m_s_side, true, false);
	}
	for (int i = static_cast<int>(Direction::Left1); i <= static_cast<int>(Direction::Left2); i++)
	{
		DrawRotaGraph(SceneSide::m_sidePosition[i].u + m_sideSize.v / 2.0f, SceneSide::m_sidePosition[i].v,
			1.0f, 3.1415f / 2.0f * 3.0f,
			m_s_side, true, false);
	}
}

void TitleScene::LoadFile()
{
	m_s_side = LoadGraph("Resources/EncanEncounter.png");
	m_s_title = LoadGraph("Resources/title.png");
	m_s_space = LoadGraph("Resources/space.png");
	m_s_ciycle = LoadGraph("Resources/circle.png");
	m_sideSize = { 1139.0f, 104.0f };
	m_titleSize = {443, 118 };
	m_spaceSize = {150, 34};
}

void TitleScene::Update()
{
	float speed = 4.0f;
	for (int i = static_cast<int>(Direction::Right1); i <= static_cast<int>(Direction::Right2); i++)
	{
		SceneSide::m_sidePosition[i].v -= speed;
		if (SceneSide::m_sidePosition[i].v < -m_sideSize.u / 2)
		{
			SceneSide::m_sidePosition[i].v = m_sideSize.u + (m_sideSize.u / 2);
		}
	}
	for (int i = static_cast<int>(Direction::Left1); i <= static_cast<int>(Direction::Left2); i++)
	{
		SceneSide::m_sidePosition[i].v += speed;
		if (SceneSide::m_sidePosition[i].v > WindowSize::Hi + m_sideSize.u / 2)
		{
			SceneSide::m_sidePosition[i].v = -((m_sideSize.u - WindowSize::Hi) + m_sideSize.u / 2);
		}
	}
}

void TitleScene::Init()
{
	for (int i = static_cast<int>(Direction::Right1); i <= static_cast<int>(Direction::Right2); i++)
	{
		SceneSide::m_sidePosition[i] = { 0, WindowSize::Wid + (m_sideSize.u * i) };
	}
	for (int i = static_cast<int>(Direction::Left1); i <= static_cast<int>(Direction::Left2); i++)
	{
		SceneSide::m_sidePosition[i] = { WindowSize::Wid - m_sideSize.v, (-m_sideSize.u / 2) - (m_sideSize.u * (i - 2)) };
	}
}

void TitleScene::Draw()
{
	DrawExtendGraph((WindowSize::Wid / 2) + Shake::GetShake().u - m_ciycleSize.u / 2.0f, WindowSize::Hi / 2 + Shake::GetShake().v - m_ciycleSize.v / 2.0f,
					(WindowSize::Wid / 2) + Shake::GetShake().u + m_ciycleSize.u / 2.0f, WindowSize::Hi / 2 + Shake::GetShake().v + m_ciycleSize.v / 2.0f,
					m_s_ciycle, true);
	for (int i = static_cast<int>(Direction::Right1); i <= static_cast<int>(Direction::Right2); i++)
	{
		DrawRotaGraph(SceneSide::m_sidePosition[i].u + m_sideSize.v / 2.0f, SceneSide::m_sidePosition[i].v,
			1.0f, 3.1415f / 2.0f,
			m_s_side, true, false);
	}
	for (int i = static_cast<int>(Direction::Left1); i <= static_cast<int>(Direction::Left2); i++)
	{
		DrawRotaGraph(SceneSide::m_sidePosition[i].u + m_sideSize.v / 2.0f, SceneSide::m_sidePosition[i].v,
			1.0f, 3.1415f / 2.0f * 3.0f,
			m_s_side, true, false);
	}
	float win = WindowSize::Wid / 2;
	float hi = WindowSize::Hi / 2;
	DrawExtendGraph(win - m_titleSize.u / 2, hi - 40 - m_titleSize.v / 2, win + m_titleSize.u / 2, hi - 40 + m_titleSize.v / 2, m_s_title, true);
	DrawExtendGraph(win - m_spaceSize.u / 2, hi + 100 - m_spaceSize.v / 2, win + m_spaceSize.u / 2, hi + 100 + m_spaceSize.v / 2, m_s_space, true);
}
