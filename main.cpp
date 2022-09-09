#include "DxLib.h"
#include "scripts/Lib/Lib.h"
#include "scripts/Player/player.h"
#include "scripts/Input/Input.h"
#include "scripts/Particle/Particle.h"
#include "scripts/Object/BaseObject.h"
#include "scripts/Scene/Scene.h"
#include "scripts/Sound/Sound.h"
#include "scripts//UI/UI.h"

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
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// ��ʃT�C�Y��ݒ�(�𑜓x�Ƃ̔䗦�Őݒ�)
	SetWindowSizeExtendRate(1.0);

	// ��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(0x20, 0x49, 0x60);

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

	// �Q�[�����[�v
	while (1)
	{
		Input::Update();

		// ��ʃN���A
		ClearDrawScreen();

		//---------  ��������v���O�������L�q  ----------//
		GetMousePoint(&mouse_x, &mouse_y);

		// �X�V����
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
			if (player.GetIsMove() && Input::isJoyLeftStickBottom())
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
					float l_leftStickDeg = 0.0f;
					l_leftStickDeg = Input::GetJoyLeftStickAngle();
					l_leftStickDeg = 180.0f / DX_PI_F * l_leftStickDeg;

					//�e���c���Ă邩�̔���
					bool isShot = player.ShotBullet();
					if (isShot)
					{
						if (rand() % 2 == 0)
						{
							ObjectManager::object1.Shot(winSizeHalf, spriteSize, l_leftStickDeg, 18.0f, BaseObject::ObjectType::ORANGE);
						}
						else
						{
							ObjectManager::object2.Shot(winSizeHalf, spriteSize, l_leftStickDeg, 18.0f, BaseObject::ObjectType::PINK);
						}
					}
				}
				BaseObject::ResetSpeed();
			}
			if (player.GetIsChangeTrigger())
			{
				BaseObject::SpeedUpdate();
			}
			BaseObject::SetIsMove(player.GetIsMove());
			FLOAT2 pos = player.GetPos();
			//�e�I�u�W�F�N�g�̍X�V
			ObjectManager::Update(pos, player.GetIsSide());
			//�p�[�e�B�N���̍X�V
			ParticleManager::Update();
			if (Input::GetKeyTrigger(KEY_INPUT_ESCAPE))
			{
				ParticleManager::AllClear();
				ObjectManager::AllClear();
				//player.Init();
				bulletUI.AllShotStart();
			}
			if (bulletUI.GetIsAllShot() && bulletUI.BulletNum() <= 0)
			{
				sceneNum = TITLE;
				player.Init();
				bulletUI.m_isAllShot = false;
			}
			bulletUI.Update(player.GetBulletNum());
		}
		// �`�揈��
		DrawGraph(0, 0, BackGraund, true);

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
			GameScene::Update();
			GameScene::Draw();
			DrawFormatString(0, 100, GetColor(0, 0, 0), "BulletNum:%d", player.GetBulletNum());
			DrawFormatString(0, 120, GetColor(0, 0, 0), "BulletNum:%d", player.GetMaxBulletNum());
			bulletUI.Draw();
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
