#include"Wave.h"
#include "../WindowsSize/WindowSize.h"
#include "../Lib/Lib.h"
#include "../Enemy/Enemy.h"
#include "../Score/Score.h"
#include "../UI/UI.h"
#include "../Player/player.h"
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

int WaveManager::WaveBorader[WaveManager::MaxWaveNum - 1] = { 500, 2000, 4000, 6500, 10000, 13500, 17500, 25000, 35000 };
int WaveManager::s_back;
FLOAT2 WaveManager::m_backSize;
float WaveManager::m_backEaseTimer;
FLOAT2 WaveManager::m_backEndSize;
bool WaveManager::isAllEnd = false;

int WaveManager::s_rule;
int WaveManager::ruleAlpha;
float WaveManager::ruleEaseTimer;
bool WaveManager::isRuleEnd;
FLOAT2 WaveManager::nowRuleSize;
FLOAT2 WaveManager::MaxRuleSize;

int WaveManager::s_change;
int WaveManager::changeAlpha;
float WaveManager::changeEaseTimer;
bool WaveManager::isChangeEnd;
FLOAT2 WaveManager::nowChangeSize;
FLOAT2 WaveManager::MaxChangeSize;

int WaveManager::s_transe;
int WaveManager::transeAlpha;
float WaveManager::transeEaseTimer;
bool WaveManager::isTranseEnd;
FLOAT2 WaveManager::nowTranseSize;
FLOAT2 WaveManager::MaxTranseSize;

int WaveManager::s_bomb;
int WaveManager::bombAlpha;
float WaveManager::bombEaseTimer;
bool WaveManager::isBombEnd;
FLOAT2 WaveManager::nowBombSize;
FLOAT2 WaveManager::MaxBombSize;

int WaveManager::s_shild;

bool WaveManager::isBombHit = false;
int WaveManager::bombTimer = 0;
bool WaveManager::isBombHeal = false;

void WaveManager::LoadFile()
{
	/*for (int i = 1; i <= 10; i++)
	{
		std::string path = "Resources/wave_" + std::to_string(i) + ".png";
		s_waves[i - 1] = LoadGraph(path.c_str());
	}*/
	s_waves[0] = LoadGraph("Resources/level_1.png");
	for (int i = 1; i < 9; i++)
	{
		s_waves[i] = LoadGraph("Resources/level_up.png");
	}
	s_waves[9] = LoadGraph("Resources/level_max.png");
	s_rule = LoadGraph("Resources/rule_guide.png");
	s_change = LoadGraph("Resources/guide_a.png");
	s_transe = LoadGraph("Resources/guide_b.png");
	s_bomb = LoadGraph("Resources/guide_rb.png");

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
	ruleAlpha = 250;
	transeAlpha = 0;
	bombAlpha = 0;
	changeAlpha = 0;
	isRuleEnd = false;
	if (waveNum == 2 && Player::GetBombCount() < 2)
	{
		EnemyManager::AddEnemy(BaseEnemy::SpeedType::Bomb);
	}

	if (waveNum == 5)
	{
		EnemyManager::AddEnemy(BaseEnemy::SpeedType::Boss);
		EnemyManager::isBoss = true;
	}
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
	if (EnemyManager::isBoss)
	{
		Score::score = WaveBorader[waveNumber] - 1;
	}

	if (Score::score >= WaveBorader[waveNumber])
	{
		WaveInit(waveNumber + 1);
	}
	else
	{
		if (waveNumber == 0)
		{
			if (EnemyManager::enemys.size() <= 0)
			{
				EnemyManager::AddEnemy(BaseEnemy::SpeedType::Normal);
			}
		}
		else if (waveNumber == 1)
		{
			if (EnemyManager::enemys.size() <= 2)
			{
				int ran = rand() % 10;
				if (ran == 0)
				{
					EnemyManager::AddEnemy(BaseEnemy::SpeedType::Midl);
					//EnemyManager::AddEnemy(BaseEnemy::SpeedType::Shild);
					//EnemyManager::AddEnemy(BaseEnemy::SpeedType::Troop);
				}
				else
				{
					EnemyManager::AddEnemy(BaseEnemy::SpeedType::Normal);
					//EnemyManager::AddEnemy(BaseEnemy::SpeedType::Shild);
					//EnemyManager::AddEnemy(BaseEnemy::SpeedType::Troop2);
				}
			}

		}
		else if (waveNumber == 2)
		{
			if (EnemyManager::enemys.size() <= 2 || rand() % 240 == 0)
			{
				for (int i = 0; i < 2; i++)
				{
					int ran = rand() % 10;
					if (ran < 2)
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Hi);
					}
					else if (ran < 8)
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Midl);
					}
					else
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Normal);
					}
				}
			}
		}
		else if (waveNumber == 3)
		{
			if (EnemyManager::enemys.size() <= 2 || rand() % 180 == 0)
			{
				for (int i = 0; i < 2; i++)
				{
					int ran = rand() % 10;
					if (ran < 2)
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Hi);
					}
					else if (ran < 8)
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Midl);
					}
					else
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Normal);
						//EnemyManager::AddEnemy(BaseEnemy::SpeedType::Troop);
					}
				}
			}
		}
		else if (waveNumber == 4)
		{
			if (EnemyManager::enemys.size() <= 2 || rand() % 180 == 0)
			{
				for (int i = 0; i < 2; i++)
				{
					int ran = rand() % 10;
					if (ran < 4)
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Hi);
					}
					else if (ran < 8)
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Midl);
					}
					else
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Normal);
					}
				}
			}
		}
		else if (waveNumber == 5)
		{
			isBombHeal = false;
			if (EnemyManager::enemys.size() <= 2 || rand() % 120 == 0)
			{
				for (int i = 0; i < 3; i++)
				{
					int ran = rand() % 10;
					if (ran < 2)
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Hi);
					}
					else if (ran < 8)
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Midl);
					}
					else
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Normal);
					}
				}
			}
			if (Player::GetBombCount() < 2 && rand() % 300 == 0 && !isBombHeal)
			{
				EnemyManager::AddEnemy(BaseEnemy::SpeedType::Bomb);
				isBombHeal = true;
			}
		}
		else if (waveNumber == 6)
		{
			if (EnemyManager::enemys.size() <= 3 || rand() % 150 == 0)
			{
				for (int i = 0; i < 3; i++)
				{
					int ran = rand() % 10;
					if (ran < 3)
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Hi);
					}
					else if (ran < 6)
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Midl);
					}
					else
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Normal);
					}
				}
			}
		}
		else if (waveNumber == 7)
		{
			if (EnemyManager::enemys.size() <= 4 || rand() % 180 == 0)
			{
				for (int i = 0; i < 3; i++)
				{
					int ran = rand() % 10;
					if (ran < 3)
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Hi);
					}
					else if (ran < 6)
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Midl);
					}
					else
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Normal);
					}
				}
			}
		}
		else if (waveNumber == 8)
		{
			if (EnemyManager::enemys.size() <= 4 || rand() % 150 == 0)
			{
				for (int i = 0; i < 4; i++)
				{
					int ran = rand() % 10;
					if (ran < 4)
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Hi);
					}
					else if (ran < 7)
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Midl);
					}
					else
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Normal);
					}
				}
			}
		}
		else if (waveNumber == 9)
		{
			if (EnemyManager::enemys.size() <= 5 || rand() % 120 == 0)
			{
				for (int i = 0; i < 5; i++)
				{
					int ran = rand() % 10;
					if (ran < 4)
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Hi);
					}
					else if (ran < 7)
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Midl);
					}
					else
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Normal);
					}
				}
			}
		}
		else if (waveNumber >= 10)
		{
			if (EnemyManager::enemys.size() <= 0 || rand() % 150 == 0)
			{
				for (int i = 0; i < 4; i++)
				{
					int ran = rand() % 5;
					if (ran == 0 || ran == 2)
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Hi);
					}
					else if (ran == 1 || ran == 3)
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Midl);
					}
					else
					{
						EnemyManager::AddEnemy(BaseEnemy::SpeedType::Normal);
					}
				}
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
		if (!isRuleEnd)
		{
			ruleEaseTimer += 0.01f;
			if (ruleEaseTimer >= 1.0f)
			{
				ruleAlpha -= 2;
				if (ruleAlpha <= 0)
				{
					ruleAlpha = 0;
					isRuleEnd = true;
					isChangeEnd = false;
					changeAlpha = 250;
					changeEaseTimer = 0;
					ruleEaseTimer = 0;
				}
			}

			float sens = 0.3f;
			FLOAT2 size = { 933.0f * sens , 174.0f * sens };
			float y = 300;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, ruleAlpha);
			DrawExtendGraph(WindowSize::Wid / 2 - size.u, WindowSize::Hi / 2 - size.v + y, WindowSize::Wid / 2 + size.u, WindowSize::Hi / 2 + size.v + y
				, s_rule, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else if (!isChangeEnd)
		{
			changeEaseTimer += 0.005f;
			if (changeEaseTimer >= 1.0f)
			{
				changeAlpha -= 2;
				if (changeAlpha <= 0)
				{
					changeAlpha = 0;
					changeEaseTimer = 0;
					isChangeEnd = true;
					isBombEnd = false;
					bombAlpha = 250;
					bombEaseTimer = 0;
				}
			}
			float sens = 0.3f;
			FLOAT2 size = { 607.0f * sens , 111.0f * sens };
			float y = 300;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, changeAlpha);
			DrawExtendGraph(WindowSize::Wid / 2 - size.u, WindowSize::Hi / 2 - size.v + y, WindowSize::Wid / 2 + size.u, WindowSize::Hi / 2 + size.v + y
				, s_change, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		/*else if (!isTranseEnd)
		{
			transeEaseTimer += 0.01f;
			if (transeEaseTimer >= 1.0f)
			{
				transeAlpha -= 2;
				if (transeAlpha <= 0)
				{
					transeAlpha = 0;
					transeEaseTimer = 0;
					isTranseEnd = true;
					isBombEnd = false;
					bombAlpha = 250;
					bombEaseTimer = 0;
				}
			}
			float sens = 0.3f;
			FLOAT2 size = { 525.0f * sens , 111.0f * sens };
			float y = 300;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, transeAlpha);
			DrawExtendGraph(WindowSize::Wid / 2 - size.u, WindowSize::Hi / 2 - size.v + y, WindowSize::Wid / 2 + size.u, WindowSize::Hi / 2 + size.v + y
				, s_transe, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}*/
		else if (!isBombEnd)
		{
			bombEaseTimer += 0.005f;
			if (bombEaseTimer >= 1.0f)
			{
				bombAlpha -= 2;
				if (bombAlpha <= 0)
				{
					bombAlpha = 0;
					bombEaseTimer = 0;
					isBombEnd = true;
				}
			}
			float sens = 0.3f;
			FLOAT2 size = { 659.0f * sens , 88.0f * sens };
			float y = 300;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, bombAlpha);
			DrawExtendGraph(WindowSize::Wid / 2 - size.u, WindowSize::Hi / 2 - size.v + y, WindowSize::Wid / 2 + size.u, WindowSize::Hi / 2 + size.v + y
				, s_bomb, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}

void WaveManager::Wave1()
{

}

void WaveManager::Wave2()
{

}
