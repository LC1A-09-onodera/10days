#pragma once
#include "DxLib.h"

class Input
{
	// �ŐV�̃L�[�{�[�h���p
	static char keys[256];
	// 1���[�v(�t���[��)�O�̃L�[�{�[�h���
	static char oldkeys[256];
	//�W���C�p�b�h���
	static XINPUT_STATE joypad;
	//��t���[���O�̃W���C�p�b�h���
	static XINPUT_STATE oldjoypad;

	static float JoyLeftDeadLine;	//�ő�32767.0f
	static float JoyRightDeadLine;	//�ő�32767.0f

public:
	static void Update();
	static bool GetKey(int f_KeyCode);
	static bool GetKeyTrigger(int f_KeyCode);

	static FLOAT2 GetJoyRightStick();				//�E�X�e�B�b�N�̓���(�����ƌX���x��)
	static float GetJoyRightStickAngle();			//�E�X�e�B�b�N�����͂���Ă���p�x���擾(radian)
	static float GetJoyRightTrigger();				//�E�g���K�[�̌X�����0~1�̒l�Ŏ擾
	static bool isJoyRightStickBottom();			//�E�X�e�B�b�N���|����Ă��邩(DeadLine����)
	static FLOAT2 GetJoyLeftStick();				//���X�e�B�b�N�̓���(�����ƌX���x��)
	static float GetJoyLeftStickAngle();			//���X�e�B�b�N�����͂���Ă���p�x���擾(radian)
	static float GetJoyLeftTrigger();				//���g���K�[�̌X�����0~1�̒l�Ŏ擾�B
	static bool isJoyLeftStickBottom();			//���X�e�B�b�N���|����Ă��邩(DeadLine����)
	static bool isJoyBottom(int JoyBottom);			//�W���C�p�b�h�{�^���̉��������擾�B
	static bool isJoyBottomTrigger(int JoyBottom);	//�W���C�p�b�h�{�^���̃g���K�[���������擾�B
	static bool isJoyBottomRelease(int JoyBottom);	//�W���C�p�b�h�{�^���̃����[�X���������擾�B
};