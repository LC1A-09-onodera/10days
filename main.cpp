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
	SetBackgroundColor(0x20, 0x49, 0x60);

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
			}
		}

		else if (sceneNum == GAME)
		{
			player.Update();
			static float angle = 0.0f;
			static int time = 0;
			if (Input::GetKeyTrigger(KEY_INPUT_SPACE))
			{
				EnemyManager::AddEnemy();
			}
			if (!player.GetIsMove())
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
					if (isShot)
					{
						if (rand() % 2 == 0)
						{
							ObjectManager::object1.Shot(winSizeHalf, spriteSize, player.GetDeg(), 18.0f, BaseObject::ObjectType::ORANGE);
						}
						else
						{
							ObjectManager::object2.Shot(winSizeHalf, spriteSize, player.GetDeg(), 18.0f, BaseObject::ObjectType::PINK);
						}
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
			if (bulletUI.GetIsAllShot() && bulletUI.BulletNum() <= 0)
			{
				sceneNum = TITLE;
				player.Init();
				bulletUI.m_isAllShot = false;
				ParticleManager::AllClear();
				ObjectManager::AllClear();
			}
			bulletUI.Update(player.GetBulletNum());
			if (Input::GetKeyTrigger(KEY_INPUT_ESCAPE))
			{
				//player.Init();
				bulletUI.AllShotStart();
			}
		}
		// 描画処理
		DrawExtendGraph(0, 0, WindowSize::Wid, WindowSize::Hi, BackGraund, true);
		ParticleManager::Draw();

		if (sceneNum == TITLE)
		{
			TitleScene::Update();
			TitleScene::Draw();
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
