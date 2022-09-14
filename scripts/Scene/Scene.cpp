#include "Scene.h"
#include "../Scroll/Scroll.h"
#include "../WindowsSize/WindowSize.h"
#include "../Lib/Lib.h"
#include "../Input/Input.h"
#include "../Enemy/Enemy.h"

int TitleScene::m_s_side;
FLOAT2 TitleScene::m_sideSize;

int GameScene::m_s_side;
FLOAT2 GameScene::m_sideSize;


int TitleScene::m_s_title;
int TitleScene::m_s_encaenca;
int TitleScene::m_s_space;
FLOAT2 TitleScene::m_titlePos;
FLOAT2 TitleScene::m_spacePos;
FLOAT2 TitleScene::m_titleSize;
FLOAT2 TitleScene::m_spaceSize;
int TitleScene::m_s_ciycle;
FLOAT2 TitleScene::m_ciycleSize = { 504, 504 };
bool TitleScene::m_isEnca = false;

FLOAT2 SceneSide::m_sidePosition[4];

int GameScene::sceneTimer = 300;

int ResultScene::ciycleR;
int ResultScene::s_space;
int ResultScene::s_score;
int ResultScene::s_number[10];
int ResultScene::m_score;
bool ResultScene::isToTitle = false;
float sent = 0.45f;
FLOAT2 ResultScene::m_numSize = { 700 / 5 * sent, 346 / 2 * sent };

void GameScene::LoadFile()
{
	m_s_side = LoadGraph("Resources/EncanEncounter.png");
	m_sideSize = { 1200.0f, 104.0f };
}

void GameScene::Update()
{
	float speed = 4.0f;
	for (int i = static_cast<int>(Direction::Right1); i <= static_cast<int>(Direction::Right2); i++)
	{
		SceneSide::m_sidePosition[i].v -= speed;
		if (SceneSide::m_sidePosition[i].v < -m_sideSize.u / 2)
		{
			SceneSide::m_sidePosition[i].v = m_sideSize.u + (m_sideSize.u / 2) + dis;
		}
	}
	for (int i = static_cast<int>(Direction::Left1); i <= static_cast<int>(Direction::Left2); i++)
	{
		SceneSide::m_sidePosition[i].v += speed;
		if (SceneSide::m_sidePosition[i].v > WindowSize::Hi + m_sideSize.u / 2)
		{
			SceneSide::m_sidePosition[i].v = -((m_sideSize.u - WindowSize::Hi) + m_sideSize.u / 2) - dis;
		}
	}
}

void GameScene::Init()
{
	for (int i = static_cast<int>(Direction::Right1); i <= static_cast<int>(Direction::Right2); i++)
	{
		SceneSide::m_sidePosition[i] = { 0, WindowSize::Wid + (m_sideSize.u * i) + (dis * i) };
	}
	for (int i = static_cast<int>(Direction::Left1); i <= static_cast<int>(Direction::Left2); i++)
	{
		SceneSide::m_sidePosition[i] = { WindowSize::Wid - m_sideSize.v, (-m_sideSize.u / 2) - (m_sideSize.u * (i - 2) + (dis * (i - 2))) };
	}
}

void GameScene::Draw()
{
	for (int i = static_cast<int>(Direction::Right1); i <= static_cast<int>(Direction::Right2); i++)
	{
		DrawRotaGraph(SceneSide::m_sidePosition[i].u + m_sideSize.v / 2.0f + Shake::GetShake().u, SceneSide::m_sidePosition[i].v + Shake::GetShake().v,
			1.0f, 3.1415f / 2.0f,
			m_s_side, true, false);
	}
	for (int i = static_cast<int>(Direction::Left1); i <= static_cast<int>(Direction::Left2); i++)
	{
		DrawRotaGraph(SceneSide::m_sidePosition[i].u + m_sideSize.v / 2.0f + Shake::GetShake().u, SceneSide::m_sidePosition[i].v + Shake::GetShake().v,
			1.0f, 3.1415f / 2.0f * 3.0f,
			m_s_side, true, false);
	}
}

void TitleScene::LoadFile()
{
	m_s_side = LoadGraph("Resources/EncanEncounter.png");
	m_s_title = LoadGraph("Resources/new_title.png");
	m_s_encaenca = LoadGraph("Resources/enca_enca.png");
	m_s_space = LoadGraph("Resources/push_guide.png");
	m_s_ciycle = LoadGraph("Resources/circle.png");
	m_sideSize = { 1139.0f, 104.0f };
	//m_titleSize = {443, 118 };
	float sen = 0.5f;
	m_titleSize = { 872.0f * sen, 325.0f * sen };
	m_spaceSize = { 552 * 0.20f, 547 * 0.20f };
}

void TitleScene::Update()
{
	GameScene::Update();
}

void TitleScene::Init()
{
	for (int i = static_cast<int>(Direction::Right1); i <= static_cast<int>(Direction::Right2); i++)
	{
		SceneSide::m_sidePosition[i] = { 0, WindowSize::Wid + (m_sideSize.u * i) + (GameScene::dis * i) };
	}
	for (int i = static_cast<int>(Direction::Left1); i <= static_cast<int>(Direction::Left2); i++)
	{
		SceneSide::m_sidePosition[i] = { WindowSize::Wid - m_sideSize.v, (-m_sideSize.u / 2) - (m_sideSize.u * (i - 2) + (GameScene::dis * (i - 2))) };
	}
	int l_rand = std::rand() % 5;
	if (l_rand == 0) { m_isEnca = true; }
	else { m_isEnca = false; }
}

void TitleScene::Draw()
{
	DrawExtendGraph((WindowSize::Wid / 2) + Shake::GetShake().u - m_ciycleSize.u / 2.0f, WindowSize::Hi / 2 + Shake::GetShake().v - m_ciycleSize.v / 2.0f,
		(WindowSize::Wid / 2) + Shake::GetShake().u + m_ciycleSize.u / 2.0f, WindowSize::Hi / 2 + Shake::GetShake().v + m_ciycleSize.v / 2.0f,
		m_s_ciycle, true);
	GameScene::Draw();
	float win = WindowSize::Wid / 2;
	float hi = WindowSize::Hi / 2;
	if (!m_isEnca)
	{
		DrawExtendGraph(win - m_titleSize.u / 2, hi - 0 - m_titleSize.v / 2, win + m_titleSize.u / 2, hi - 0 + m_titleSize.v / 2, m_s_title, true);
	}
	else
	{
		DrawExtendGraph(win - m_titleSize.u / 2, hi + 10 - m_titleSize.v / 2, win + m_titleSize.u / 2, hi - 40 + m_titleSize.v / 2, m_s_encaenca, true);
	}
	DrawExtendGraph(win - m_spaceSize.u / 2, hi + 120 - m_spaceSize.v / 2, win + m_spaceSize.u / 2, hi + 120 + m_spaceSize.v / 2, m_s_space, true);
}

void TitleScene::SetRand()
{
	int l_rand = std::rand() % 5;
	if (l_rand == 0) { m_isEnca = true; }
	else { m_isEnca = false; }
}

void ResultScene::Draw()
{
	if (isToTitle)
	{
		TitleScene::Draw();
	}
	DrawCircleAA(WindowSize::Wid / 2, WindowSize::Hi / 2, ciycleR, 128, GetColor(13, 13, 13));

	if (!isToTitle)
	{
		DrawExtendGraph(WindowSize::Wid / 2 - (340 / 2 * 0.5f) - 200, WindowSize::Hi / 2 - (75 / 2 * 0.5f),
			WindowSize::Wid / 2 + (340 / 2 * 0.5f) - 200, WindowSize::Hi / 2 + (75 / 2 * 0.5f), s_score, true);
		float x = -32;
		int score[6] = { m_score / 100000 % 10,m_score / 10000 % 10,m_score / 1000 % 10,m_score / 100 % 10,m_score / 10 % 10, m_score % 10 };
		for (int i = 0; i < 6; i++)
		{
			DrawExtendGraph(WindowSize::Wid / 2 - (m_numSize.u / 2) + x, WindowSize::Hi / 2 - (m_numSize.v / 2),
				WindowSize::Wid / 2 + (m_numSize.u / 2) + x, WindowSize::Hi / 2 + (m_numSize.v / 2), s_number[score[i]], true);
			x += m_numSize.u;
		}
		DrawExtendGraph(WindowSize::Wid / 2 - (552 / 2 * 0.2f), WindowSize::Hi / 2 - (509 / 2 * 0.2f) + 150,
			WindowSize::Wid / 2 + (552 / 2 * 0.2f), WindowSize::Hi / 2 + (509 / 2 * 0.2f) + 150, s_space, true);
	}

}

void ResultScene::Update()
{
	if (!isToTitle)
	{
		ciycleR = Easeing::EaseInQuad(ciycleR, MaxCiycle, 0.3f);
		EnemyManager::CiycleInc();
	}
	else
	{
		ResultToTitle();
	}
}


void ResultScene::Init(int f_score)
{
	m_score = f_score;
	ciycleR = 0;
	isToTitle = false;
}

void ResultScene::LoadFile()
{
	s_space = LoadGraph("Resources/push_guide_w.png");
	s_score = LoadGraph("Resources/score.png");
	LoadDivGraph("Resources/numbers_w.png", 10, 5, 2, 700 / 5, 346 / 2, s_number);
}

void ResultScene::ResultToTitle()
{
	ciycleR = Easeing::EaseInQuad(ciycleR, 0, 0.3f);
}
