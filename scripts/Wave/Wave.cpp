#include"Wave.h"
#include "../WindowsSize/WindowSize.h"
#include "../Lib/Lib.h"
#include "../Enemy/Enemy.h"
#include "../Score/Score.h"
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

void WaveManager::LoadFile()
{
	for (int i = 1; i <= 10; i++)
	{
		std::string path = "Resources/wave_" + std::to_string(i) + ".png";
		s_waves[i - 1] = LoadGraph(path.c_str());
	}
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
}

void WaveManager::Update()
{
	m_backEaseTimer += 0.01f;
	m_backSize = Easeing::EaseInQuad(m_backSize, m_backEndSize, m_backEaseTimer);
	if (m_backEaseTimer > 1.0f)
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
	DrawBox(0, WindowSize::Hi / 2 - m_backSize.v / 2, WindowSize::Wid, WindowSize::Hi / 2 + m_backSize.v / 2, GetColor(123, 204, 41), true);
	DrawExtendGraph(m_position.u - m_size.u / 2, m_position.v - m_size.v / 2, m_position.u + m_size.u / 2, m_position.v + m_size.v / 2,
		s_waves[waveNumber], true);
}

void WaveManager::Wave1()
{

}

void WaveManager::Wave2()
{

}
