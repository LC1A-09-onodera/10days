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


// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "10days";

// ウィンドウ横幅
const int WIN_WIDTH = 1280;

// ウィンドウ縦幅
const int WIN_HEIGHT = 720;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
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
		if (sceneNum == TITLE)
		{
			if (Input::GetKeyTrigger(KEY_INPUT_SPACE) || Input::isJoyBottomTrigger(XINPUT_BUTTON_A))
			{
				sceneNum = GAME;
				TowerHP::HP = TowerHP::MaxHP;
				bulletUI.m_isAllShot = false;
			}
		}

		else if (sceneNum == GAME)
		{
			player.Update();
			static float angle = 0.0f;
			static int time = 0;
			EnemyManager::CiycleDec();
			EnemyManager::CiycleDec();
			if (Input::GetKeyTrigger(KEY_INPUT_SPACE)||Input::isJoyBottom(XINPUT_BUTTON_X))
			{
				EnemyManager::AddEnemy();
			}
			int nowBullet = bulletUI.m_bullets.size();
			if (!player.GetIsMove() && nowBullet >= player.GetBulletNum())
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
					bool isShot = player.ShotBullet();
					if (isShot && !bulletUI.m_isAllShot)
					{
						if (rand() % 2 == 0)
						{
							ObjectManager::object1.Shot(winSizeHalf, spriteSize, player.GetDeg(), 18.0f, BaseObject::ObjectType::ORANGE);
						}
						else
						{
							ObjectManager::object2.Shot(winSizeHalf, spriteSize, player.GetDeg(), 18.0f, BaseObject::ObjectType::PINK);
						}
						StopSoundMem(SoundManager::shotBullet);
						PlaySoundMem(SoundManager::shotBullet, DX_PLAYTYPE_BACK);
					}
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

			//エネミーとリフレクターの判定
			for (auto& x : EnemyManager::enemys)
			{
				if (!x->m_isReturn)
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

					//円周上なら
					const float l_checkDiff = 20.0f;
					const float l_diff = fabsf(player.GetStageReflectorRad() - l_len);
					if (l_diff < l_checkDiff)
					{
						//中心から見た敵の角度を算出
						FLOAT2 l_vec;
						l_vec.u = l_diffX / l_len;
						l_vec.v = l_diffY / l_len;
						float l_enemyDeg = atan2f(l_vec.v, l_vec.u);
						l_enemyDeg = 180.0f / DX_PI_F * l_enemyDeg;

						//中心から見た自機の角度を算出
						float l_playerDeg = 180.0f / DX_PI_F * player.GetReflectorRad();

						//何度まで当たるか
						const float l_hitDeg = 20.0f;
						const float l_degDiff = fabsf(l_playerDeg - 90.0f - l_enemyDeg);

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

			bulletUI.Update(player.GetBulletNum());

			if (bulletUI.GetIsAllShot() && bulletUI.BulletNum() <= 0)
			{
				sceneNum = RESULT;
				player.Init();
				bulletUI.m_isAllShot = false;
				ParticleManager::AllClear();
				ObjectManager::AllClear();
				ResultScene::Init(12345);
			}

			if (TowerHP::HP <= 0)
			{
				//player.Init();
				bulletUI.m_isAllShot = false;
				EnemyManager::AllDelete();
				bulletUI.AllShotStart();
			}
		}
		// 描画処理
		DrawExtendGraph(0, 0, WindowSize::Wid, WindowSize::Hi, BackGraund, true);
		ParticleManager::Draw();

		if (sceneNum == TITLE)
		{
			player.Draw();
			TitleScene::Update();
			TitleScene::Draw();
			EnemyManager::Draw();
		}

		else if (sceneNum == GAME)
		{
			player.Draw();
			ObjectManager::Draw();
			GameScene::Draw();

			EnemyManager::Draw();
			DrawFormatString(0, 100, GetColor(0, 0, 0), "BulletNum:%d", player.GetBulletNum());
			DrawFormatString(0, 120, GetColor(0, 0, 0), "BulletNum:%d", player.GetMaxBulletNum());
			DrawFormatString(400, 100, GetColor(0, 0, 0), "Score:%d", Score::GetScore());
			bulletUI.Draw();
			TowerHP::Draw();
		}
		else if (sceneNum == RESULT)
		{
			if (Input::GetKeyTrigger(KEY_INPUT_SPACE))
			{
				ResultScene::isToTitle = true;
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
