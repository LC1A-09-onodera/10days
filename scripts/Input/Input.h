#pragma once
#include "DxLib.h"

class Input
{
	// �ŐV�̃L�[�{�[�h���p
	static char keys[256];
	// 1���[�v(�t���[��)�O�̃L�[�{�[�h���
	static char oldkeys[256];
public:
	static void Update();
	static bool GetKey(int f_KeyCode);
	static bool GetKeyTrigger(int f_KeyCode);
};