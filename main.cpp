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
	TowerHP::s_HP = LoadGraph("Resources/hp.png");
	WaveManager::LoadFile();
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
				WaveManager::WaveInit(0);
			}
		}

		else if (sceneNum == GAME)
		{
			WaveManager::Update();
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

			//�G�l�~�[�ƃ��t���N�^�[�̔���
			for (auto& x : EnemyManager::enemys)
			{
				if (!x->m_isReturn)
				{
					//�G�ƒ��S�̋���
					FLOAT2 l_halfWinSize = player.GetHalfWinSize();
					FLOAT2 l_enemyPos = x->m_position;
					float l_diffX = l_enemyPos.u - l_halfWinSize.u;
					float l_diffY = l_enemyPos.v - l_halfWinSize.v;
					float l_len = sqrtf(
						powf(l_diffX, 2.0f) +
						powf(l_diffY, 2.0f)
					);

					//�~����Ȃ�
					const float l_checkDiff = 20.0f;
					const float l_diff = fabsf(player.GetStageReflectorRad() - l_len);
					if (l_diff < l_checkDiff)
					{
						//���S���猩���G�̊p�x���Z�o
						FLOAT2 l_vec;
						l_vec.u = l_diffX / l_len;
						l_vec.v = l_diffY / l_len;
						float l_enemyDeg = atan2f(l_vec.v, l_vec.u);
						l_enemyDeg = 180.0f / DX_PI_F * l_enemyDeg;

						//���S���猩�����@�̊p�x���Z�o
						float l_playerDeg = 180.0f / DX_PI_F * player.GetReflectorRad();

						//���x�܂œ����邩
						const float l_hitDeg = 20.0f;
						const float l_degDiff = fabsf(l_playerDeg - 90.0f - l_enemyDeg);

						//�͈͓�
						if (l_degDiff < l_hitDeg)
						{
							x->m_isReturn = true;
							x->HitShiled();
							player.ReflectorHit(x->m_position);
						}
						//0~359�x�̃P�A
						else
						{
							if (l_playerDeg < l_hitDeg || l_enemyDeg < l_hitDeg)
							{
								if (l_playerDeg < l_hitDeg) { l_playerDeg += 359.9f; }
								if (l_enemyDeg < l_hitDeg) { l_enemyDeg += 359.9f; }

								//�͈͓�
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
			if (WaveManager::isAllEnd)
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
			if (WaveManager::isAllEnd)
			{
				TowerHP::Draw();
			}
			bulletUI.Draw();
			player.Draw();
			ObjectManager::Draw();
			GameScene::Draw();

			EnemyManager::Draw();
			DrawFormatString(0, 100, GetColor(0, 0, 0), "BulletNum:%d", player.GetBulletNum());
			DrawFormatString(0, 120, GetColor(0, 0, 0), "BulletNum:%d", player.GetMaxBulletNum());
			DrawFormatString(400, 100, GetColor(0, 0, 0), "Score:%d", Score::GetScore());
			WaveManager::Draw();
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
