#include "player.h"
#include "../Scroll/Scroll.h"
#include "../Sound/Sound.h"
#include "../Wave/Wave.h"
int Player::m_bomb;
bool Player::m_isShotBomb;

void Player::Init()
{
	//位置,変
	m_position = {
		C_HALF_WID,
		C_HALF_HEI + C_STAGE_RAD - C_PLAYER_RAD };
	m_gravity.v = 0.9f;
	m_scrollStartLine = { m_winSize.u / 2, 0 };

	m_hitPos = { 0,0 };
	m_start_pos = { 0,0 };
	m_vec = { 0,0 };
	m_end_pos = { 0,0 };
	m_reflector_pos = {
		C_HALF_WID,
		C_HALF_HEI + C_STAGE_REFLECTOR_RAD };
	m_mode = SHOT;
	m_stage_rad = C_STAGE_RAD;
	m_bulletNum = C_BULLET_INIT_VAL;
	m_bomb = C_BOMB_INIT_VAL;
	m_maxBulletNum = m_bulletNum;
	m_stage_rad = C_STAGE_RAD;
	m_rad = 1.0f;
	m_easeTimer = 0.0f;
	m_deg = 90.0f;
	m_reflector_rad = DX_PI_F / 2.0f;
	m_reflector_size = 0.0f;
	m_bombLength = 0.0f;
	m_isMove = false;
	m_stageSize = { 504, 504 };
	m_isReload = false;
	m_isChange = false;
	m_isReflectorHit = false;
	m_isChangeMode = false;
	m_isShotBomb = false;
	m_inGame = false;

	//New
	m_isDash = false;
	m_isDashVec = false;
	m_dushFrameCount = 0;
	m_addSpeed = 0.0f;
	m_refPos2 = {
		C_HALF_WID,
		C_HALF_HEI - C_STAGE_REFLECTOR_RAD };
	m_refRad2 = m_reflector_rad + DX_PI_F;
}

void Player::Update()
{
	if (Shake::GetPower().u > 0.0f)
	{
		FLOAT2 l_shakePower = { -0.05f,-0.05f };
		Shake::AddShakePower(l_shakePower);
	}

	//通常時
	if (!m_isMove)
	{
		//ボム以外
		if (!m_isShotBomb)
		{
			//自機の方向ベクトルを計算
			FLOAT2 l_diff = { 0,0 };
			l_diff.u = m_position.u - C_HALF_WID;
			l_diff.v = m_position.v - C_HALF_HEI;
			float l_len = sqrtf(
				powf(l_diff.u, 2.0f) +
				powf(l_diff.v, 2.0f));
			FLOAT2 l_vec = { 0,0 };
			l_vec.u = l_diff.u / l_len;
			l_vec.v = l_diff.v / l_len;
			float l_pRad = atan2f(-l_vec.v, -l_vec.u);
			if (l_pRad < 0.0f)
			{
				l_pRad += DX_PI_F * 2;
			}

			//左スティックが倒されている時のみ(コントローラー以外も対応させろ！)
			if (Input::isPadConnect())
			{
				//m_deg = 180.0f / DX_PI_F * l_pRad;

				if (m_dushFrameCount >= DushFrame)
				{
					m_addSpeed = 0.0f;
					m_isDash = false;
				}

				if (m_dushFrameCount >= DushDelay)
				{
					if (Input::isJoyBottomTrigger(XINPUT_BUTTON_B) &&
						m_mode == Mode::REFLECTION)
					{
						float l_stAng = Input::GetJoyLeftStickAngle();
						if (l_stAng < 0.0f)
						{
							l_stAng += DX_PI_F * 2.0f;
						}
						l_stAng = l_stAng * 180.0f / DX_PI_F;
						if (l_stAng >= 300.0f || l_stAng <= 60.0f)
						{
							m_isDashVec = true;
						}
						else if (l_stAng >= 120.0f && l_stAng <= 240.0f)
						{
							m_isDashVec = false;
						}
						else
						{
							m_isDashVec = false;
						}

						m_dushFrameCount = 0;
						m_isDash = true;
					}
				}
				else
				{
					m_dushFrameCount++;
				}

				if (Input::isJoyLeftStickBottom() || m_isDash)
				{
					//自機の位置算出正規化
					//m_vec = Input::GetJoyLeftStick();
					//float l_len = sqrtf(powf(m_vec.u, 2.0f) + powf(m_vec.v, 2.0f));
					//m_vec.u /= l_len;
					//m_vec.v /= l_len * -1;

					//float l_pAngle = 180.0f / DX_PI_F * atan2f(l_vec.v, l_vec.u);
					//float l_sAngle = 180.0f / DX_PI_F * atan2f(m_vec.v, m_vec.u);
					//if (l_pAngle < 0.0f) { l_pAngle += 360.0f; }
					//if (l_sAngle < 0.0f) { l_sAngle += 360.0f; }
					//float l_nearArc = RotateEarliestArc(l_pAngle, l_sAngle);

					//FLOAT2 l_nearVec = { 0,0 };
					////要修正
					////float l_speed = 0.0f;
					////if (l_nearArc < 0.0f) { l_speed = -C_MAX_MOVE_SPEED; }
					////else if (l_nearArc > 0.0f) { l_speed = C_MAX_MOVE_SPEED; }
					////if (fabsf(l_nearArc) < C_MAX_MOVE_SPEED)
					////{
					//	//if (l_nearArc < 0.0f) { l_speed = -l_nearArc; }
					//	//else if (l_nearArc > 0.0f) { l_speed = l_nearArc; }
					////}

					//float l_arc = l_nearArc / 30.0f;
					//if (m_isDash)
					//{
					//	if (m_addSpeed < MaxAddSpeed)
					//	{
					//		m_addSpeed += AddSpeed;
					//	}
					//	l_arc = 10.0f;
					//}
					//else
					//{
					//	if (m_addSpeed > 1.0f)
					//	{
					//		m_addSpeed -= SubSpeed;
					//	}
					//}

					//float l_rad = (l_pAngle + l_arc) * DX_PI_F / 180.0f;
					//l_nearVec.u = cosf(l_rad);
					//l_nearVec.v = sinf(l_rad);

					//m_position.u = l_nearVec.u * (C_STAGE_RAD - C_PLAYER_RAD) + C_HALF_WID;
					//m_position.v = l_nearVec.v * (C_STAGE_RAD - C_PLAYER_RAD) + C_HALF_HEI;

					////リフレクター
					//m_reflector_pos.u = l_nearVec.u * C_STAGE_REFLECTOR_RAD + C_HALF_WID;
					//m_reflector_pos.v = l_nearVec.v * C_STAGE_REFLECTOR_RAD + C_HALF_HEI;
					//m_reflector_rad = l_rad;
					//if (m_reflector_rad < 0.0f)
					//{
					//	m_reflector_rad += DX_PI_F * 2.0f;
					//}

					//新移動
					if (m_isDash)
					{
						if (m_addSpeed < MaxAddSpeed)
						{
							m_addSpeed += AddSpeed;
						}
					}

					//ダッシュ以外は通常移動
					float l_stAng = Input::GetJoyLeftStickAngle();
					if (l_stAng < 0.0f)
					{
						l_stAng += DX_PI_F * 2.0f;
					}
					l_stAng = l_stAng * 180.0f / DX_PI_F;

					if (m_isDash)
					{
						if (m_isDashVec)
						{
							m_deg += 2.0f + m_addSpeed;
						}
						else
						{
							m_deg -= 2.0f + m_addSpeed;
						}
					}
					else
					{
						float l_stAng = Input::GetJoyLeftStickAngle();
						if (l_stAng < 0.0f)
						{
							l_stAng += DX_PI_F * 2.0f;
						}
						l_stAng = l_stAng * 180.0f / DX_PI_F;

						if (l_stAng >= 300.0f || l_stAng <= 60.0f)
						{
							m_deg += 2.0f;
						}
						else if (l_stAng >= 120.0f && l_stAng <= 240.0f)
						{
							m_deg -= 2.0f;
						}
					}

					if (m_deg >= 360.0f)
					{
						m_deg = 0.0f;
					}
					if (m_deg < 0.0f)
					{
						float l_sub = fabsf(m_deg);
						m_deg = 360.0f - l_sub;
					}

					//自機の位置算出正規化
					float l_rad = m_deg / 180.0f * DX_PI_F;
					FLOAT2 l_nearVec = { 0,0 };
					l_nearVec.u = cosf(l_rad);
					l_nearVec.v = sinf(l_rad);

					m_position.u = l_nearVec.u * (C_STAGE_RAD - C_PLAYER_RAD) + C_HALF_WID;
					m_position.v = l_nearVec.v * (C_STAGE_RAD - C_PLAYER_RAD) + C_HALF_HEI;

					m_refPos2.u = -l_nearVec.u * C_STAGE_REFLECTOR_RAD + C_HALF_WID;
					m_refPos2.v = -l_nearVec.v * C_STAGE_REFLECTOR_RAD + C_HALF_HEI;

					//リフレクター
					m_reflector_pos.u = l_nearVec.u * C_STAGE_REFLECTOR_RAD + C_HALF_WID;
					m_reflector_pos.v = l_nearVec.v * C_STAGE_REFLECTOR_RAD + C_HALF_HEI;
					m_reflector_rad = l_rad;
					if (m_reflector_rad < 0.0f)
					{
						m_reflector_rad += DX_PI_F * 2.0f;
					}

					m_refRad2 = m_reflector_rad + DX_PI_F;
					if (m_refRad2 > DX_PI_F * 2.0f)
					{
						m_refRad2 -= DX_PI_F * 2.0f;
					}
				}
			}

			//キーボード
			else
			{
				if (Input::GetKey(KEY_INPUT_RIGHT))
				{
					m_deg -= 2.0f;
				}
				if (Input::GetKey(KEY_INPUT_LEFT))
				{
					m_deg += 2.0f;
				}
				if (m_deg >= 360.0f)
				{
					m_deg = 0.0f;
				}
				if (m_deg < 0.0f)
				{
					float l_sub = fabsf(m_deg);
					m_deg = 360.0f - l_sub;
				}
				//自機の位置算出正規化
				float l_rad = m_deg / 180.0f * DX_PI_F;
				FLOAT2 l_nearVec = { 0,0 };
				l_nearVec.u = cosf(l_rad);
				l_nearVec.v = sinf(l_rad);

				m_position.u = l_nearVec.u * (C_STAGE_RAD - C_PLAYER_RAD) + C_HALF_WID;
				m_position.v = l_nearVec.v * (C_STAGE_RAD - C_PLAYER_RAD) + C_HALF_HEI;

				//リフレクター
				m_refPos2.u = -l_nearVec.u * C_STAGE_REFLECTOR_RAD + C_HALF_WID;
				m_refPos2.v = -l_nearVec.v * C_STAGE_REFLECTOR_RAD + C_HALF_HEI;

				m_reflector_pos.u = l_nearVec.u * C_STAGE_REFLECTOR_RAD + C_HALF_WID;
				m_reflector_pos.v = l_nearVec.v * C_STAGE_REFLECTOR_RAD + C_HALF_HEI;
				m_reflector_rad = l_rad;
				if (m_reflector_rad < 0.0f)
				{
					m_reflector_rad += DX_PI_F * 2.0f;
				}

				m_refRad2 = m_reflector_rad + DX_PI_F;
				if (m_refRad2 > DX_PI_F * 2.0f)
				{
					m_refRad2 -= DX_PI_F * 2.0f;
				}
			}

			//縦断入力
			/*if (Input::GetKeyTrigger(KEY_INPUT_Z) ||
				Input::isJoyBottomTrigger(XINPUT_BUTTON_B))*/
				/*if (false)
				{
					StopSoundMem(SoundManager::transe);
					PlaySoundMem(SoundManager::transe, DX_PLAYTYPE_BACK);

					m_bulletNum = m_maxBulletNum;
					m_start_pos = m_position;
					l_vec.u *= -1.0f;
					l_vec.v *= -1.0f;
					m_end_pos.u = l_vec.u * (C_STAGE_RAD - C_PLAYER_RAD) + C_HALF_WID;
					m_end_pos.v = l_vec.v * (C_STAGE_RAD - C_PLAYER_RAD) + C_HALF_HEI;
					m_vec = l_vec;
					m_isMove = true;
				}*/

				//モード変更
			if (!m_isChangeMode && !m_isDash)
			{
				if (Input::GetKeyTrigger(KEY_INPUT_Z) ||
					Input::isJoyBottomTrigger(XINPUT_BUTTON_A))
				{
					StopSoundMem(SoundManager::weponChange);
					PlaySoundMem(SoundManager::weponChange, DX_PLAYTYPE_BACK);
					m_isChangeMode = true;
				}
			}
			else if (m_isChangeMode)
			{
				const float l_addTimer = C_ADD_TIMER * 2.0f;
				if (m_mode == REFLECTION)
				{
					if (m_reflector_size > 0.0f)
					{
						m_reflector_size -= l_addTimer;
					}
					if (m_reflector_size <= 0.0f)
					{
						m_reflector_size = 0.0f;
						m_mode = SHOT;
						m_isChangeMode = false;
					}
				}
				else
				{
					if (m_reflector_size < 1.0f)
					{
						m_reflector_size += l_addTimer;
					}
					if (m_reflector_size >= 1.0f)
					{
						m_reflector_size = 1.0f;
						m_bulletNum = m_maxBulletNum;
						m_mode = REFLECTION;
						m_isChangeMode = false;
					}
				}
			}

			//ボム発射
			if (!m_isShotBomb && m_bomb > 0)
			{
				if (Input::GetKeyTrigger(KEY_INPUT_SPACE) ||
					Input::isJoyBottomTrigger(XINPUT_BUTTON_RIGHT_SHOULDER))
				{
					//WaveManager::isBombHit = true;
					FLOAT2 l_addPower{ 7.0f,7.0f };
					Shake::AddShakePower(l_addPower);
					StopSoundMem(SoundManager::BombShot);
					PlaySoundMem(SoundManager::BombShot, DX_PLAYTYPE_BACK);
					m_isShotBomb = true;
					m_bomb--;
				}
			}

			//リフレクターヒット時
			if (m_isReflectorHit)
			{
				FLOAT2 l_shakePower = { 3.0f,3.0f };
				Shake::SetShakePower(l_shakePower);
				m_isReflectorHit = false;
			}

			//射撃モード
			if (m_mode == SHOT)
			{
				//自動リロード
				if (m_bulletNum == 0)
				{
					m_bulletNum = m_maxBulletNum;
				}
			}
		}
	}

	//縦断移動中
	else
	{
		//タイマー加算
		if (m_easeTimer < 1.0f) { m_easeTimer += C_ADD_TIMER; }

		//移動処理
		m_position.u = (m_end_pos.u - m_start_pos.u) * easeInOutSine(m_easeTimer) + m_start_pos.u;
		m_position.v = (m_end_pos.v - m_start_pos.v) * easeInOutSine(m_easeTimer) + m_start_pos.v;
		if (m_easeTimer >= 1.0f)
		{
			m_easeTimer = 0.0f;
			m_isMove = false;
		}

		//自機拡縮処理
		const float l_addTimer = C_ADD_TIMER * 2.0f;
		if (!m_isChange)
		{
			if (m_rad > 0.0f)
			{
				m_rad -= l_addTimer;
			}
			if (m_rad < 0.0f) { m_rad = 0.0f; }
			if (m_easeTimer >= 0.5f)
			{
				m_reflector_pos.u = m_vec.u * C_STAGE_REFLECTOR_RAD + C_HALF_WID;
				m_reflector_pos.v = m_vec.v * C_STAGE_REFLECTOR_RAD + C_HALF_HEI;
				m_reflector_rad -= DX_PI_F;
				m_isChange = true;
			}
		}
		else
		{
			if (m_rad < 1.0f)
			{
				m_rad += l_addTimer;
			}
			if (m_rad > 1.0f) { m_rad = 1.0f; }
			if (!m_isMove)
			{
				m_isChange = false;
			}
		}
	}

	//ヒット時演出
	for (auto itr = m_effects.begin(); itr != m_effects.end(); ++itr)
	{
		if (itr->timer < 1.0f)
		{
			itr->timer += C_ADD_TIMER * 1.2f;
		}
		if (itr->timer > 0.3f)
		{
			itr->alpha -= 10;
		}
		if (itr->timer > 1.0f) { itr->timer = 1.0f; }
		if (!itr->isDraw) { itr->isDraw = true; }
		else { itr->isDraw = false; }

		itr->r = easeInOutSine(itr->timer);

		if (itr->alpha <= 0)
		{
			m_effects.erase(itr);
			break;
		}
	}

	//ボムのLength計算
	if (m_isShotBomb)
	{
		m_bombLength += C_ADD_BOMB_LENGTH;

		if (m_bombLength > m_winSize.u)
		{
			m_bombLength = 0.0f;
			m_isShotBomb = false;
		}
	}
}

void Player::OtherUpdate()
{
	if (m_inGame)
	{
		//サイズ調整
		if (m_rad < 1.0f)
		{
			m_rad += 0.1f;
		}
	}
	else
	{
		m_rad = 0.0f;

		//コントローラー
		if (Input::isPadConnect())
		{
			//左スティックが倒されている時のみ(コントローラー以外も対応させろ！)
			if (Input::isJoyLeftStickBottom())
			{
				//新移動
				float l_stAng = Input::GetJoyLeftStickAngle();
				if (l_stAng < 0.0f)
				{
					l_stAng += DX_PI_F * 2.0f;
				}
				l_stAng = l_stAng * 180.0f / DX_PI_F;

				if (l_stAng >= 300.0f || l_stAng <= 60.0f)
				{
					m_deg += 2.0f;
				}
				if (l_stAng >= 120.0f && l_stAng <= 240.0f)
				{
					m_deg -= 2.0f;
				}

				if (m_deg >= 360.0f)
				{
					m_deg = 0.0f;
				}
				if (m_deg < 0.0f)
				{
					float l_sub = fabsf(m_deg);
					m_deg = 360.0f - l_sub;
				}

				//自機の位置算出正規化
				float l_rad = m_deg / 180.0f * DX_PI_F;
				FLOAT2 l_nearVec = { 0,0 };
				l_nearVec.u = cosf(l_rad);
				l_nearVec.v = sinf(l_rad);

				m_position.u = l_nearVec.u * (C_STAGE_RAD - C_PLAYER_RAD) + C_HALF_WID;
				m_position.v = l_nearVec.v * (C_STAGE_RAD - C_PLAYER_RAD) + C_HALF_HEI;

				//リフレクター
				m_refPos2.u = -l_nearVec.u * C_STAGE_REFLECTOR_RAD + C_HALF_WID;
				m_refPos2.v = -l_nearVec.v * C_STAGE_REFLECTOR_RAD + C_HALF_HEI;

				m_reflector_pos.u = l_nearVec.u * C_STAGE_REFLECTOR_RAD + C_HALF_WID;
				m_reflector_pos.v = l_nearVec.v * C_STAGE_REFLECTOR_RAD + C_HALF_HEI;
				m_reflector_rad = l_rad;
				if (m_reflector_rad < 0.0f)
				{
					m_reflector_rad += DX_PI_F * 2.0f;
				}

				m_refRad2 = m_reflector_rad + DX_PI_F;
				if (m_refRad2 > DX_PI_F * 2.0f)
				{
					m_refRad2 -= DX_PI_F * 2.0f;
				}
			}
		}

		//キーボード
		else
		{
			if (Input::GetKey(KEY_INPUT_RIGHT))
			{
				m_deg -= 2.0f;
			}
			if (Input::GetKey(KEY_INPUT_LEFT))
			{
				m_deg += 2.0f;
			}
			//自機の位置算出正規化
			float l_rad = m_deg / 180.0f * DX_PI_F;
			FLOAT2 l_nearVec = { 0,0 };
			l_nearVec.u = cosf(l_rad);
			l_nearVec.v = sinf(l_rad);

			m_position.u = l_nearVec.u * (C_STAGE_RAD - C_PLAYER_RAD) + C_HALF_WID;
			m_position.v = l_nearVec.v * (C_STAGE_RAD - C_PLAYER_RAD) + C_HALF_HEI;

			//リフレクター
			m_refPos2.u = -l_nearVec.u * C_STAGE_REFLECTOR_RAD + C_HALF_WID;
			m_refPos2.v = -l_nearVec.v * C_STAGE_REFLECTOR_RAD + C_HALF_HEI;

			m_reflector_pos.u = l_nearVec.u * C_STAGE_REFLECTOR_RAD + C_HALF_WID;
			m_reflector_pos.v = l_nearVec.v * C_STAGE_REFLECTOR_RAD + C_HALF_HEI;
			m_reflector_rad = l_rad;
			if (m_reflector_rad < 0.0f)
			{
				m_reflector_rad += DX_PI_F * 2.0f;
			}

			m_refRad2 = m_reflector_rad + DX_PI_F;
			if (m_refRad2 > DX_PI_F * 2.0f)
			{
				m_refRad2 -= DX_PI_F * 2.0f;
			}
		}
	}
}

void Player::Draw()
{
	//自機
	float l_addRad = 0;
	float l_pSize = 1.0f;
	if (m_reflector_size >= 0.5f)
	{
		l_addRad = DX_PI_F;
		l_pSize = m_reflector_size;
	}
	else
	{
		l_pSize = (1.0f - m_reflector_size);
	}
	DrawRotaGraph(
		static_cast<int>(m_position.u + Shake::GetShake().u),
		static_cast<int>(m_position.v + Shake::GetShake().v),
		static_cast<double>(m_rad) * l_pSize * 0.2,
		static_cast<double>(m_reflector_rad) + l_addRad - DX_PI_F / 2.0,
		m_s_player,
		true
	);

	//リフレクターから発生するパーティクル
	if (m_mode == REFLECTION)
	{
		const int l_reflector_len = 180;
		const float l_len = rand() % l_reflector_len;
		const float l_rand = l_len - l_reflector_len / 2.0f;
		float l_reflector_posX = m_reflector_pos.u + l_rand * cosf(m_reflector_rad);
		float l_reflector_posY = m_reflector_pos.v + l_rand * sinf(m_reflector_rad);
		FLOAT2 l_pos = { l_reflector_posX,l_reflector_posY };
		FLOAT2 startSize = { 20.0f, 20.0f };
		FLOAT2 endSize = { 2.0f, 2.0f };
		ParticleManager::smpParticle.StayParticle(l_pos, startSize, endSize, 2, 60);

		//New
		l_reflector_posX = m_refPos2.u + l_rand * cosf(m_reflector_rad);
		l_reflector_posY = m_refPos2.v + l_rand * sinf(m_reflector_rad);
		l_pos = { l_reflector_posX,l_reflector_posY };
		ParticleManager::smpParticle.StayParticle(l_pos, startSize, endSize, 2, 60);
	}

	//リフレクター(薄い)
	const float l_reflectorSize = 0.22f;

	DrawRotaGraph(
		static_cast<int>(m_reflector_pos.u + Shake::GetShake().u),
		static_cast<int>(m_reflector_pos.v + Shake::GetShake().v),
		static_cast<double>(m_rad) * l_reflectorSize,
		static_cast<double>(m_reflector_rad),
		m_s_reflector_alpha,
		true
	);

	DrawRotaGraph(
		static_cast<int>(m_refPos2.u + Shake::GetShake().u),
		static_cast<int>(m_refPos2.v + Shake::GetShake().v),
		static_cast<double>(m_rad) * l_reflectorSize,
		static_cast<double>(m_reflector_rad),
		m_s_reflector_alpha,
		true
	);

	//リフレクター
	DrawRotaGraph(
		static_cast<int>(m_reflector_pos.u + Shake::GetShake().u),
		static_cast<int>(m_reflector_pos.v + Shake::GetShake().v),
		static_cast<double>(m_rad) * m_reflector_size * l_reflectorSize,
		static_cast<double>(m_reflector_rad),
		m_s_reflector,
		true
	);

	//New
	DrawRotaGraph(
		static_cast<int>(m_refPos2.u + Shake::GetShake().u),
		static_cast<int>(m_refPos2.v + Shake::GetShake().v),
		static_cast<double>(m_rad) * m_reflector_size * l_reflectorSize,
		static_cast<double>(m_reflector_rad),
		m_s_reflector,
		true
	);

	//ステージ
	DrawExtendGraph(
		(WindowSize::Wid / 2) + Shake::GetShake().u - m_stageSize.u / 2.0f,
		(WindowSize::Hi / 2) + Shake::GetShake().v - m_stageSize.v / 2.0f,
		(WindowSize::Wid / 2) + Shake::GetShake().u + m_stageSize.u / 2.0f,
		(WindowSize::Hi / 2) + Shake::GetShake().v + m_stageSize.v / 2.0f,
		m_s_stage,
		true
	);

	//ヒット時演出
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	for (auto itr = m_effects.begin(); itr != m_effects.end(); ++itr)
	{
		if (itr->isDraw)
		{
			DrawRotaGraph(
				itr->pos.u + Shake::GetShake().u,
				itr->pos.v + Shake::GetShake().v,
				static_cast<double>(itr->r) * 0.5,
				0.0f,
				m_s_reflector_hit,
				true
			);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ボム
	if (m_isShotBomb)
	{
		DrawRotaGraph(
			C_HALF_WID + Shake::GetShake().u,
			C_HALF_HEI + Shake::GetShake().v,
			static_cast<double>(m_bombLength) / 254.0,
			0.0,
			m_s_bomb_effect,
			true
		);
	}

	//debug
	//float hoge = Shake::GetPowerX();
	//float hoge2 = Shake::GetPowerY();
	//DrawFormatString(50, 20, GetColor(0, 0, 0), "ShakeX:%f", hoge);
	//DrawFormatString(50, 40, GetColor(0, 0, 0), "ShakeY:%f", hoge2);
	//DrawFormatString(50, 60, GetColor(0, 0, 0), "RefRad:%f", m_reflector_rad);
}

void Player::OtherDraw()
{
	if (!m_inGame)
	{
		//自機
		float l_addRad = 0;
		float l_pSize = 1.0f;
		DrawRotaGraph(
			static_cast<int>(m_position.u + Shake::GetShake().u),
			static_cast<int>(m_position.v + Shake::GetShake().v),
			l_pSize * 0.2,
			static_cast<double>(m_reflector_rad) + l_addRad - DX_PI_F / 2.0,
			m_s_player,
			true
		);
	}
}

void Player::LoadFile()
{
	m_s_player = LoadGraph("Resources/player.png");
	m_sprite = LoadGraph("Resources/particle.png");
	m_s_stage = LoadGraph("Resources/circle.png");
	m_s_reflector = LoadGraph("Resources/new_reflecter.png");
	m_s_reflector_alpha = LoadGraph("Resources/new_reflecter_alpha.png");
	m_s_reflector_hit = LoadGraph("Resources/hit_effect.png");
	m_s_bomb_effect = LoadGraph("Resources/bomb_effect.png");
	Init();
}

bool Player::GetIsMove()
{
	return m_isMove;
}

void Player::AddForce()
{

}

void Player::AttachForce()
{

}

void Player::ReflectorHit(FLOAT2& hitPos)
{
	Effects l_effects;
	l_effects.pos = hitPos;
	l_effects.r = 0;
	l_effects.alpha = 255;
	l_effects.timer = 0.0f;
	l_effects.isDraw = true;
	m_effects.push_back(l_effects);

	m_isReflectorHit = true;
}

bool Player::IsShotBomb()
{
	return m_isShotBomb;
}

bool Player::ShotBullet()
{
	if (m_bulletNum > 0)
	{
		m_bulletNum--;
		return true;
	}
	return false;
}
