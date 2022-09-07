#include "Scene.h"


int GameScene::m_s_backBround;
int GameScene::m_s_side;
FLOAT2 GameScene::m_backPosition;
FLOAT2 GameScene::m_backSize;
FLOAT2 GameScene::m_sidePosition[4];
FLOAT2 GameScene::m_sideSize;
void GameScene::LoadFile()
{
	m_s_side = LoadGraph("Resources/EncanEncounter.png");
	m_sideSize = {1139.0f, 104.0f};
}

void GameScene::Update()
{
	for (int i = static_cast<int>(Direction::Right1); i <= static_cast<int>(Direction::Right2); i++)
	{
		m_sidePosition[i].v -= 2.0f;
		if (m_sidePosition[i].v < -m_sideSize.u)
		{
			m_sidePosition[i].v = m_sideSize.u - m_sideSize.u / 4.0f;
		}
	}
	for (int i = static_cast<int>(Direction::Left1); i <= static_cast<int>(Direction::Left2); i++)
	{
		m_sidePosition[i].v += 2.0f;
		if (m_sidePosition[i].v > m_sideSize.u)
		{
			m_sidePosition[i].v = -(m_sideSize.u - m_sideSize.u / 4.0f);
		}
	}
}

void GameScene::Init()
{
	for (int i = static_cast<int>(Direction::Right1); i <= static_cast<int>(Direction::Right2); i++)
	{
		m_sidePosition[i] = {0, 1139.0f * (i + 1)};
	}
	for (int i = static_cast<int>(Direction::Left1); i <= static_cast<int>(Direction::Left2); i++)
	{
		m_sidePosition[i] = { 1280 - m_sideSize.v, -1139.0f * (i - 1)};
	}
}

void GameScene::Draw()
{
	for (int i = static_cast<int>(Direction::Right1); i <= static_cast<int>(Direction::Right2); i++)
	{
		DrawRotaGraph(m_sidePosition[i].u + m_sideSize.v / 2.0f, m_sidePosition[i].v,
			1.0f, 3.1415f / 2.0f,
			m_s_side, true, false);
	}
	for (int i = static_cast<int>(Direction::Left1); i <= static_cast<int>(Direction::Left2); i++)
	{
		DrawRotaGraph(m_sidePosition[i].u + m_sideSize.v / 2.0f, m_sidePosition[i].v,
			1.0f, 3.1415f / 2.0f * 3.0f,
			m_s_side, true, false);
	}
}
