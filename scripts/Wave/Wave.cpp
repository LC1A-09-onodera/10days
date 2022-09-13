#include"Wave.h"
#include "../WindowsSize/WindowSize.h"
#include "../Lib/Lib.h"
#include "../Enemy/Enemy.h"
#include "../Score/Score.h"
#include "../UI/UI.h"

#include <string>

int WaveManager::waveNumber = 0;
int WaveManager::s_waves[WaveManager::MaxWaveNum];
bool WaveManager::isWaveDirectionStart = false;
bool WaveManager::isWaveDirectionEnd = false;
bool WaveManager::isToNextWave = false;
float WaveManager::easeTimer = 0;
FLOAT2 WaveManager::m_size = { 659, 177 };
FLOAT2 WaveManager::m_position = { WindowSize::Wid + WaveManager::m_size.u , WindowSize::Hi / 2 };
FLOAT2 WaveManager::m_endPosition;
int WaveManager::stopTimer = 0;
bool WaveManager::isStopEnd = false;

int WaveManager::WaveBorader[WaveManager::MaxWaveNum] = { 10, 40, 120, 200, 300, 500, 800, 1200, 2000, 3000 };
int WaveManager::s_back;
FLOAT2 WaveManager::m_backSize;
float WaveManager::m_backEaseTimer;
FLOAT2 WaveManager::m_backEndSize;
bool WaveManager::isAllEnd = false;

int WaveManager::s_rule;
int WaveManager::alpha;

void WaveManager::LoadFile()
{
	for (int i = 1; i <= 10; i++)
	{
		std::string path = "Resources/wave_" + std::to_string(i) + ".png";
		s_waves[i - 1] = LoadGraph(path.c_str());
	}
	s_rule = LoadGraph("Resources/rule_guide.png");
}

void WaveManager::WaveInit(int waveNum)
{
	waveNumber = waveNum;
	isWaveDirectionStart = true;
	isWaveDirectionEnd = false;
	easeTimer = 0.0f;
	stopTimer = 0;
	m_position = { WindowSize::Wid + m_size.u / 2, WindowSize::Hi / 2 };
	m_endPosition = { WindowSize::Wid / 2, WindowSize::Hi / 2 };
	m_backSize = { WindowSize::Wid, 0 };
	m_backEndSize = { WindowSize::Wid, 200 };
	m_backEaseTimer = 0.0f;
	isAllEnd = false;
	isStopEnd = false;
	ScoreUI::isIncDec = false;
	TowerHP::isIncDec = false;
	alpha = 250;
}

void WaveManager::Update()
{
	m_backEaseTimer += 0.01f;
	m_backSize = Easeing::EaseInQuad(m_backSize, m_backEndSize, m_backEaseTimer);
	if (m_backEaseTimer > 1.0f || (m_backSize.v < 1 && isWaveDirectionEnd) || (m_backSize.v > 198 && !isWaveDirectionEnd))
	{
		easeTimer += 0.05f;
		m_position = Easeing::EaseInQuad(m_position, m_endPosition, easeTimer);
		if (isWaveDirectionEnd)
		{
			isAllEnd = true;
		}
		if (easeTimer >= 1.0f)
		{
			m_position = Easeing::EaseInQuad(m_position, m_endPosition, 1.0f);
			stopTimer++;
			if (stopTimer > stopTime && !isStopEnd)
			{
				isStopEnd = true;
				easeTimer = 0.0f;
				m_endPosition = { -m_size.u, WindowSize::Hi / 2 };
			}
			else if (isStopEnd)
			{
				isWaveDirectionStart = false;
				isWaveDirectionEnd = true;
				m_backEaseTimer = 0;
				m_backEndSize = { WindowSize::Wid, 0.0f };
			}
		}
		if (m_backSize.v < 1)
		{
			m_backSize.v = 0;
			ScoreUI::isIncDec = true;
			TowerHP::isIncDec = true;
		}
	}

	if (Score::score > WaveBorader[waveNumber])
	{
		WaveInit(waveNumber + 1);
	}
	else
	{
		if (waveNumber == 0)
		{
			if (EnemyManager::enemys.size() <= 0)
			{
				EnemyManager::AddEnemy();
			}
		}
		else
		{
			if (rand() % (300 / waveNumber) == 0)
			{
				EnemyManager::AddEnemy();
				EnemyManager::AddEnemy();
			}
		}
	}
}

void WaveManager::Draw()
{
	//DrawBox(0, WindowSize::Hi / 2 - m_backSize.v / 2, WindowSize::Wid, WindowSize::Hi / 2 + m_backSize.v / 2, GetColor(123, 204, 41), true);
	DrawExtendGraph(m_position.u - m_size.u / 2, m_position.v - m_size.v / 2, m_position.u + m_size.u / 2, m_position.v + m_size.v / 2,
		s_waves[waveNumber], true);
	if (waveNumber == 0)
	{
		alpha -= 1;
		if (alpha <= 0)
		{
			alpha = 0;
		}
		float sens = 0.3f;
		FLOAT2 size = { 933.0f * sens , 174.0f * sens };
		float y = 300;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawExtendGraph(WindowSize::Wid / 2 - size.u, WindowSize::Hi / 2 - size.v + y, WindowSize::Wid / 2 + size.u, WindowSize::Hi / 2 + size.v + y
						, s_rule, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void WaveManager::Wave1()
{

}

void WaveManager::Wave2()
{

}
