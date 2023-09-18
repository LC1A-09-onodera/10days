#include "DxLib.h"
#include "scripts/Lib/Lib.h"
#include "scripts/Player/player.h"
#include "scripts/Input/Input.h"
#include "scripts/Particle/Particle.h"
#include "scripts/Object/BaseObject.h"
#include "scripts/Scene/Scene.h"
#include "scripts/Sound/Sound.h"
#include "scripts//UI/UI.h"
#include "scripts/WindowsSize/WindowSize.h"
#include "scripts/Enemy/Enemy.h"
#include "scripts/Score/Score.h"
#include "scripts/Wave/Wave.h"
#include "scripts/Scroll/Scroll.h"
#include <string>
#include <time.h>

// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "円環エンカウンター";

// ウィンドウ横幅
const int WIN_WIDTH = 1280;

// ウィンドウ縦幅
const int WIN_HEIGHT = 720;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	srand((unsigned int)time(NULL));
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// ウィンドウサイズを手動では変更させず、
	// かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// タイトルを変更
	SetMainWindowText(TITLE);

	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WindowSize::Wid, WindowSize::Hi, 32);

	// 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	// 画面の背景色を設定する
	SetBackgroundColor(0xF0, 0xF0, 0xF0);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み
	int Particle1 = LoadGraph("Resources/particle.png");
	FLOAT2 particlePos = { 0 };
	float easeTime = 0;

	int BackGraund = LoadGraph("Resources/back.png");

	int vignetteB = LoadGraph("Resources/vignette_brack.png");
	int vignetteR = LoadGraph("Resources/vignette_red.png");

	// ゲームループで使う変数の宣言
	int mouse_x;
	int mouse_y;
	FLOAT2 mousePos;
	Player player;
	player.LoadFile();
	ObjectManager::LoadFile();
	ParticleManager::LoadFile();
	GameScene::LoadFile();
	GameScene::Init();
	TitleScene::LoadFile();
	TitleScene::Init();
	SoundManager::LoadFile();

	enum SceneNum
	{
		TITLE,
		GAME,
		RESULT,
	};

	SceneNum sceneNum = TITLE;

	BulletUI bulletUI;
	bulletUI.LoadFile();
	bulletUI.AddBullet();
	BaseEnemy::LoadFile();
	ResultScene::LoadFile();
	TowerHP::s_HP = LoadGraph("Resources/hp.png");
	for (int i = 1; i < 4; i++)
	{
		std::string path = "Resources/heart" + std::to_string(i) + ".png";
		TowerHP::s_hart[i] = LoadGraph(path.c_str());
	}
	WaveManager::LoadFile();

	ScoreUI::LoadFile();
	ScoreUI::Init();

	// ゲームループ
	while (1)
	{
		Input::Update();

		// 画面クリア
		ClearDrawScreen();

		//---------  ここからプログラムを記述  ----------//
		GetMousePoint(&mouse_x, &mouse_y);
		mousePos = { (float)mouse_x, (float)mouse_y };

		// 更新処理

		//内部管理
		player.OtherUpdate();

		if (sceneNum == TITLE)
		{
			FLOAT2 l_resetShakePower{ -50.0f,-50.0f };
			Shake::AddShakePower(l_resetShakePower);
			player.InGame(false);

			if (Input::GetKeyTrigger(KEY_INPUT_SPACE) || Input::isJoyBottomTrigger(XINPUT_BUTTON_A))
			{
				sceneNum = GAME;
				WaveManager::isBombHeal = false;
				TowerHP::HP = TowerHP::MaxHP;
				bulletUI.m_isAllShot = false;
				WaveManager::WaveInit(0);
				StopSoundMem(SoundManager::select);
				PlaySoundMem(SoundManager::select, DX_PLAYTYPE_BACK);
				SoundManager::BGMVol = 180;
				ChangeVolumeSoundMem(SoundManager::BGMVol, SoundManager::BGM);
				StopSoundMem(SoundManager::BGM);
				PlaySoundMem(SoundManager::BGM, DX_PLAYTYPE_BACK);
				player.InGame(true);
				TitleScene::SetRand();
				//Score::score = 34999;
			}
		}

		else if (sceneNum == GAME)
		{
			player.Update();
			WaveManager::Update();
			if (CheckSoundMem(SoundManager::BGM) == 0)
			{
				PlaySoundMem(SoundManager::BGM, DX_PLAYTYPE_BACK);
			}
			if (!WaveManager::isBombHit)
			{
				static float angle = 0.0f;
				static int time = 0;
				EnemyManager::CiycleDec();
				int nowBullet = bulletUI.m_bullets.size();
				if (!player.GetIsMove() && nowBullet >= player.GetBulletNum() && player.GetMode() == player.SHOT && !player.IsShotBomb())
				{
					time++;
					if (time > 2)
					{
						time = 0;
						angle = rand() % 360;
						if (angle >= 360.0f)
						{
							angle = 0.0f;
						}
						FLOAT2 winSizeHalf = { player.GetPos().u,  player.GetPos().v };
						FLOAT2 spriteSize = { 30.0f, 30.0f };

						//弾が残ってるかの判定
						static int ShotDistance = 0;
						ShotDistance++;
						/*if (ShotDistance % 2 == 0)
						{*/
						bool isShot = player.ShotBullet();
						if (isShot && !bulletUI.m_isAllShot)
						{

							ObjectManager::object1.Shot(winSizeHalf, spriteSize, player.GetDeg(), 18.0f, BaseObject::ObjectType::ORANGE);
							StopSoundMem(SoundManager::shotBullet);
							PlaySoundMem(SoundManager::shotBullet, DX_PLAYTYPE_BACK);
						}
						//}
					}
					BaseObject::ResetSpeed();
				}
				BaseObject::SetIsMove(player.GetIsMove());
				FLOAT2 pos = player.GetPos();
				//各オブジェクトの更新
				ObjectManager::Update(pos, true);
				//パーティクルの更新
				ParticleManager::Update();
				//エネミーの更新
				EnemyManager::Update();
				GameScene::Update();
				ScoreUI::Update(Score::GetScore());
				//エネミーとリフレクターの判定
				for (auto& x : EnemyManager::enemys)
				{
					//敵と中心の距離
					FLOAT2 l_halfWinSize = player.GetHalfWinSize();
					FLOAT2 l_enemyPos = x->m_position;
					float l_diffX = l_enemyPos.u - l_halfWinSize.u;
					float l_diffY = l_enemyPos.v - l_halfWinSize.v;
					float l_len = sqrtf(
						powf(l_diffX, 2.0f) +
						powf(l_diffY, 2.0f)
					);

					if (player.IsShotBomb())
					{
						if (l_len < player.GetBombLength() && l_len > player.GetBombLength() - 40)
						{
							if (!x->isDelete)
							{
								x->HitShiled();
								if (x->speedType == BaseEnemy::SpeedType::Bomb)
								{
									x->isBombErase = true;
								}
							}
						}
					}

					//決め打ち(内側に向かってる途中)
					if (x->m_state == 2)
					{
						//中心から見た敵の角度を算出
						FLOAT2 l_vec;
						l_vec.u = l_diffX / l_len;
						l_vec.v = l_diffY / l_len;
						float l_enemyDeg = atan2f(l_vec.v, l_vec.u);
						l_enemyDeg = 180.0f / DX_PI_F * l_enemyDeg;

						/*-----敵停止処理-----*/
						if (player.GetMode() == player.SHOT)
						{
							//中心から見た自機の角度を算出
							float l_playerDeg = player.GetDeg();

							//何度まで当たるか
							const float l_hitDeg = 30.0f;
							float l_degDiff = fabsf(l_playerDeg - l_enemyDeg);

							//範囲内
							if (l_degDiff < l_hitDeg)
							{
								//x->isStop = true;
							}
							//0~359度のケア
							else
							{
								if (l_playerDeg < l_hitDeg || l_enemyDeg < l_hitDeg)
								{
									if (l_playerDeg < l_hitDeg) { l_playerDeg += 359.9f; }
									if (l_enemyDeg < l_hitDeg) { l_enemyDeg += 359.9f; }
									l_degDiff = fabsf(l_playerDeg - l_enemyDeg);

									//範囲内
									if (l_degDiff < l_hitDeg)
									{
										//x->isStop = true;
									}
									else
									{
										x->isStop = false;
									}
								}
								else
								{
									x->isStop = false;
								}
							}
						}

						/*-----敵殺す処理-----*/
						else
						{
							//判定を入れる距離
							const float l_checkDiff = 100.0f;
							const float l_diff = fabsf(player.GetStageReflectorRad() - l_len);
							if (l_diff < l_checkDiff)
							{
								//中心から見た自機の角度を算出
								float l_playerDeg = 180.0f / DX_PI_F * player.GetReflectorRad();

								//何度まで当たるか
								const float l_hitDeg = 5.0f;
								float l_degDiff = fabsf(l_playerDeg - l_enemyDeg);

								//範囲内
								if (l_degDiff < l_hitDeg)
								{
									x->m_isReturn = true;
									x->HitShiled();
									player.ReflectorHit(x->m_position);
								}
								//0~359度のケア
								else
								{
									if (l_playerDeg < l_hitDeg || l_enemyDeg < l_hitDeg)
									{
										if (l_playerDeg < l_hitDeg) { l_playerDeg += 359.9f; }
										if (l_enemyDeg < l_hitDeg) { l_enemyDeg += 359.9f; }
										l_degDiff = fabsf(l_playerDeg - l_enemyDeg);

										//範囲内
										if (l_degDiff < l_hitDeg)
										{
											x->m_isReturn = true;
											x->HitShiled();
											player.ReflectorHit(x->m_position);
										}
									}
								}
							}
						}
					}
				}

				//if (WaveManager::isAllEnd)
				{
					bulletUI.Update(player.GetBulletNum());
				}
				if (bulletUI.GetIsAllShot() && bulletUI.BulletNum() <= 0)
				{
					sceneNum = RESULT;
					player.Init();
					bulletUI.m_isAllShot = false;
					ParticleManager::AllClear();
					ObjectManager::AllClear();
					ResultScene::Init(Score::GetScore());
					EnemyManager::AllDelete();
					Score::score = 0;

					//GameScene::Init();
				}

				if (TowerHP::HP <= 0 || Input::GetKeyTrigger(KEY_INPUT_ESCAPE))
				{
					//player.Init();
					bulletUI.m_isAllShot = false;
					bulletUI.AllShotStart();
				}
			}
			else
			{
				WaveManager::bombTimer++;
				if (WaveManager::bombTimer >= WaveManager::BombHitTime)
				{
					WaveManager::isBombHit = false;
					WaveManager::bombTimer = 0;
				}
			}
			if (Score::score >= 999999)
			{
				Score::score = 999999;
			}
		}
		// 描画処理
		DrawExtendGraph(0, 0, WindowSize::Wid, WindowSize::Hi, BackGraund, true);
		ParticleManager::Draw();
		player.OtherDraw();

		if (sceneNum == TITLE)
		{
			player.Draw();
			TitleScene::Update();
			TitleScene::Draw();
			EnemyManager::Draw();
		}

		else if (sceneNum == GAME)
		{
			WaveManager::Draw();

			TowerHP::Draw();
			ScoreUI::Draw();

			bulletUI.Draw();
			player.Draw();
			ObjectManager::Draw();
			GameScene::Draw();

			EnemyManager::Draw();


			ParticleManager::scoreParitcle.Draw();

		}
		else if (sceneNum == RESULT)
		{
			player.InGame(false);
			SoundManager::BGMVol -= 2;
			if (SoundManager::BGMVol <= 0)
			{
				SoundManager::BGMVol = 0;
			}
			ChangeVolumeSoundMem(SoundManager::BGMVol, SoundManager::BGM);
			if (Input::GetKeyTrigger(KEY_INPUT_SPACE) || Input::isJoyBottomTrigger(XINPUT_BUTTON_A))
			{
				ResultScene::isToTitle = true;
				ScoreUI::Init();
				StopSoundMem(SoundManager::select);
				PlaySoundMem(SoundManager::select, DX_PLAYTYPE_BACK);
			}
			player.Draw();
			ObjectManager::Draw();
			GameScene::Draw();
			TitleScene::Update();
			EnemyManager::Draw();
			bulletUI.Draw();
			ResultScene::Update();
			if (ResultScene::ciycleR < 2)
			{
				sceneNum = TITLE;
			}
			ResultScene::Draw();
		}
		
		
		//255がそれを使う
		static int alpha = 255;
		/*if (!EnemyManager::isBoss)
		{
			if (alpha < 255)
			{
				alpha++;
			}
		}
		else
		{
			if (alpha > 0)
			{
				alpha--;
			}
		}*/
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawExtendGraph(0, 0,WindowSize::Wid, WindowSize::Hi, vignetteB, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		/*SetDrawBlendMode(DX_BLENDMODE_ALPHA, (255 - alpha));
		DrawExtendGraph(0, 0,WindowSize::Wid, WindowSize::Hi, vignetteR, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);*/

		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();

		// 20ミリ秒待機(疑似60FPS)
		WaitTimer(20);

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1)
		{
			break;
		}

		// ESCキーが押されたらループから抜ける
		/*if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}*/
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}
