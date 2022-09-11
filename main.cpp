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


// �E�B���h�E�̃^�C�g���ɕ\�����镶����
const char TITLE[] = "10days";

// �E�B���h�E����
const int WIN_WIDTH = 1280;

// �E�B���h�E�c��
const int WIN_HEIGHT = 720;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// �E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	// �E�B���h�E�T�C�Y���蓮�ł͕ύX�������A
	// ���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// �^�C�g����ύX
	SetMainWindowText(TITLE);

	// ��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�(���j�^�[�̉𑜓x�ɍ��킹��)
	SetGraphMode(WindowSize::Wid, WindowSize::Hi, 32);

	// ��ʃT�C�Y��ݒ�(�𑜓x�Ƃ̔䗦�Őݒ�)
	SetWindowSizeExtendRate(1.0);

	// ��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(0xF0, 0xF0, 0xF0);

	// DXlib�̏�����
	if (DxLib_Init() == -1) { return -1; }

	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���
	int Particle1 = LoadGraph("Resources/particle.png");
	FLOAT2 particlePos = { 0 };
	float easeTime = 0;

	int BackGraund = LoadGraph("Resources/back.png");

	// �Q�[�����[�v�Ŏg���ϐ��̐錾
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
	// �Q�[�����[�v
	while (1)
	{
		Input::Update();

		// ��ʃN���A
		ClearDrawScreen();

		//---------  ��������v���O�������L�q  ----------//
		GetMousePoint(&mouse_x, &mouse_y);
		mousePos = { (float)mouse_x, (float)mouse_y };

		// �X�V����
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
			if (Input::GetKeyTrigger(KEY_INPUT_SPACE))
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

					//�e���c���Ă邩�̔���
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
			//�e�I�u�W�F�N�g�̍X�V
			ObjectManager::Update(pos, true);
			//�p�[�e�B�N���̍X�V
			ParticleManager::Update();
			//�G�l�~�[�̍X�V
			EnemyManager::Update();
			GameScene::Update();
			
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
		// �`�揈��
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
		//---------  �����܂łɃv���O�������L�q  ---------//
		// (�_�u���o�b�t�@)����
		ScreenFlip();

		// 20�~���b�ҋ@(�^��60FPS)
		WaitTimer(20);

		// Windows�V�X�e�����炭�������������
		if (ProcessMessage() == -1)
		{
			break;
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		/*if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}*/
	}
	// Dx���C�u�����I������
	DxLib_End();

	// ����I��
	return 0;
}
