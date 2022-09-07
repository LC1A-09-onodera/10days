#include "Input.h"
#include <cmath>

// 最新のキーボード情報用
char Input::keys[256] = { 0 };
// 1ループ(フレーム)前のキーボード情報
char Input::oldkeys[256] = { 0 };

XINPUT_STATE Input::joypad;
XINPUT_STATE Input::oldjoypad;

void Input::Update()
{
	// 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
	for (int i = 0; i < 256; i++)
	{
		oldkeys[i] = keys[i];
	}
	// 最新のキーボード情報を取得
	GetHitKeyStateAll(keys);
	//前フレームのジョイパッド情報を保存
	oldjoypad = joypad;
	//最新のジョイパッド情報を取得
	GetJoypadXInputState(DX_INPUT_PAD1, &joypad);
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

FLOAT2 Input::GetJoyRightStick()
{
	FLOAT2 stick;
	//デッドラインを設けてX軸方向の傾き具合を取得する。
	if (fabs(joypad.ThumbRX) <= 3000) {
		stick.u = 0;
	}
	else {
		stick.u = joypad.ThumbRX / 32767;
	}
	//デッドラインを設けてY軸方向の傾き具合を取得する。
	if (fabs(joypad.ThumbRY) <= 3000) {
		stick.v = 0;
	}
	else {
		stick.u = joypad.ThumbRY / 32767;
	}
	return stick;
}

float Input::GetJoyRightStickAngle() {
	FLOAT2 stick;
	//デッドラインを設けてX軸方向の傾き具合を取得する。
	if (fabs(joypad.ThumbRX) <= 1000) {
		stick.u = 0;
	}
	else {
		stick.u = joypad.ThumbRX / 32767.0f;
	}
	//デッドラインを設けてY軸方向の傾き具合を取得する。
	if (fabs(joypad.ThumbRY) <= 1000) {
		stick.v = 0;
	}
	else {
		stick.v = joypad.ThumbRY / 32767.0f;
	}
	return -atan2(stick.v, stick.u);
}

float Input::GetJoyRightTrigger() {
	float trigger = 0;
	//デッドラインを設ける。
	if (fabsf(joypad.RightTrigger) <= 30.0f) {
		trigger = 0;
	}
	else {
		trigger = joypad.RightTrigger / 255.0f;
	}
	return trigger;
}

FLOAT2 Input::GetJoyLeftStick()
{
	FLOAT2 stick;
	//デッドラインを設けてX軸方向の傾き具合を取得する。
	if (fabs(joypad.ThumbLX) <= 3000) {
		stick.u = 0;
	}
	else {
		stick.u = joypad.ThumbLX / 32767.0f;
	}
	//デッドラインを設けてY軸方向の傾き具合を取得する。
	if (fabs(joypad.ThumbLY) <= 3000) {
		stick.v = 0;
	}
	else {
		stick.v = joypad.ThumbLY / 32767.0f;
	}
	return stick;
}

float Input::GetJoyLeftStickAngle() {
	FLOAT2 stick;
	//デッドラインを設けてX軸方向の傾き具合を取得する。
	if (fabs(joypad.ThumbLX) <= 1000) {
		stick.u = 0;
	}
	else {
		stick.u = joypad.ThumbLX / 32767.0f;
	}
	//デッドラインを設けてY軸方向の傾き具合を取得する。
	if (fabs(joypad.ThumbLY) <= 1000) {
		stick.v = 0;
	}
	else {
		stick.v = joypad.ThumbLY / 32767.0f;
	}
	return -atan2(stick.v, stick.u);
}

float Input::GetJoyLeftTrigger() {
	float trigger = 0;
	//デッドラインを設ける。
	if (fabsf(joypad.LeftTrigger) <= 30.0f) {
		trigger = 0;
	}
	else {
		trigger = joypad.LeftTrigger / 255.0f;
	}
	return trigger;
}

bool Input::isJoyBottom(int JoyBottom) {
	return joypad.Buttons[JoyBottom];
}

bool Input::isJoyBottomTrigger(int JoyBottom) {
	return joypad.Buttons[JoyBottom] && !oldjoypad.Buttons[JoyBottom];
}

bool Input::isJoyBottomRelease(int JoyBottom) {
	return !joypad.Buttons[JoyBottom] && oldjoypad.Buttons[JoyBottom];
}
