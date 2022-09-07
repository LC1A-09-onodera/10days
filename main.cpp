#include "DxLib.h"
#include "scripts/Lib/Lib.h"
#include "scripts/Player/player.h"
#include "scripts/Input/Input.h"
#include "scripts/Particle/Particle.h"
#include "scripts/Object/BaseObject.h"

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

	// �Q�[�����[�v�Ŏg���ϐ��̐錾
	int mouse_x;
	int mouse_y;
	Player player;
	player.LoadFile();
	Particle par;
	par.LoadFile("Resources/particle.png");
	ObjectManager::LoadFile();
	ParticleManager::LoadFile();
	// �Q�[�����[�v
	while (1)
	{
		Input::Update();

		// ��ʃN���A
		ClearDrawScreen();

		//---------  ��������v���O�������L�q  ----------//
		GetMousePoint(&mouse_x, &mouse_y);

		// �X�V����
		player.Update();
		par.Update();
		// �`�揈��
		//player.Draw();
		par.Draw();
		static float angle = 0.0f;
		static int time = 0;
		if (player.GetIsMove())
		{
			time++;
			if (time > 15)
			{
				time = 0;
				angle = rand() % 360;
				if (angle >= 360.0f)
				{
					angle = 0.0f;
				}
				//FLOAT2 winSizeHalf = { mouse_x,  mouse_y };
				FLOAT2 winSizeHalf = { player.GetPos().u,  player.GetPos().v };
				FLOAT2 spriteSize = { 30.0f, 30.0f };
				if (rand() % 2 == 0)
				{
					ObjectManager::object1.Shot(winSizeHalf, spriteSize, angle, 18.0f, BaseObject::ObjectType::ORANGE);
				}
				else
				{
					ObjectManager::object2.Shot(winSizeHalf, spriteSize, angle, 18.0f, BaseObject::ObjectType::PINK);
				}
			}
		}
		FLOAT2 pos = player.GetPos();
		ObjectManager::Update(pos, player.GetIsSide());

		ParticleManager::Update();

		// �`�揈��
		player.Draw();
		ObjectManager::Draw();

		ParticleManager::Draw();

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
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}
	}
	// Dx���C�u�����I������
	DxLib_End();

	// ����I��
	return 0;
}
