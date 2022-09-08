#include "Scene.h"

int TitleScene::m_s_side;
FLOAT2 TitleScene::m_sidePosition[4];
FLOAT2 TitleScene::m_sideSize;

int GameScene::m_s_side;
FLOAT2 GameScene::m_sidePosition[4];
FLOAT2 GameScene::m_sideSize;
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
		m_sidePosition[i].v -= speed;
		if (m_sidePosition[i].v < -m_sideSize.u / 2)
		{
			m_sidePosition[i].v = m_sideSize.u + (m_sideSize.u / 2);
		}
	}
	for (int i = static_cast<int>(Direction::Left1); i <= static_cast<int>(Direction::Left2); i++)
	{
		m_sidePosition[i].v += speed;
		if (m_sidePosition[i].v > 720 + m_sideSize.u / 2)
		{
			m_sidePosition[i].v = -((m_sideSize.u - 720) + m_sideSize.u / 2);
		}
	}
}

void GameScene::Init()
{
	for (int i = static_cast<int>(Direction::Right1); i <= static_cast<int>(Direction::Right2); i++)
	{
		m_sidePosition[i] = {0, 1280 + (m_sideSize.u * i)};
	}
	for (int i = static_cast<int>(Direction::Left1); i <= static_cast<int>(Direction::Left2); i++)
	{
		m_sidePosition[i] = { 1280 - m_sideSize.v, (-m_sideSize.u / 2) - (m_sideSize.u * (i - 2))};
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

void TitleScene::LoadFile()
{
	m_s_side = LoadGraph("Resources/EncanEncounter.png");
	m_sideSize = { 1139.0f, 104.0f };
}

void TitleScene::Update()
{
	float speed = 4.0f;
	for (int i = static_cast<int>(Direction::Right1); i <= static_cast<int>(Direction::Right2); i++)
	{
		m_sidePosition[i].v -= speed;
		if (m_sidePosition[i].v < -m_sideSize.u / 2)
		{
			m_sidePosition[i].v = m_sideSize.u + (m_sideSize.u / 2);
		}
	}
	for (int i = static_cast<int>(Direction::Left1); i <= static_cast<int>(Direction::Left2); i++)
	{
		m_sidePosition[i].v += speed;
		if (m_sidePosition[i].v > 720 + m_sideSize.u / 2)
		{
			m_sidePosition[i].v = -((m_sideSize.u - 720) + m_sideSize.u / 2);
		}
	}
}

void TitleScene::Init()
{
	for (int i = static_cast<int>(Direction::Right1); i <= static_cast<int>(Direction::Right2); i++)
	{
		m_sidePosition[i] = { 0, 1280 + (m_sideSize.u * i) };
	}
	for (int i = static_cast<int>(Direction::Left1); i <= static_cast<int>(Direction::Left2); i++)
	{
		m_sidePosition[i] = { 1280 - m_sideSize.v, (-m_sideSize.u / 2) - (m_sideSize.u * (i - 2)) };
	}
}

void TitleScene::Draw()
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
