#include "Input.h"
// �ŐV�̃L�[�{�[�h���p
char Input::keys[256] = { 0 };
// 1���[�v(�t���[��)�O�̃L�[�{�[�h���
char Input::oldkeys[256] = { 0 };

void Input::Update()
{
	// �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
	for (int i = 0; i < 256; i++)
	{
		oldkeys[i] = keys[i];
	}
	// �ŐV�̃L�[�{�[�h�����擾
	GetHitKeyStateAll(keys);
}

bool Input::GetKey(int f_KeyCode)
{
	return CheckHitKey(f_KeyCode);
}

bool Input::GetKeyTrigger(int f_KeyCode)
{
	bool result = !oldkeys[f_KeyCode] && keys[f_KeyCode];
	return result;
}
