#pragma once
#include "DxLib.h"

class Input
{
	// 最新のキーボード情報用
	static char keys[256];
	// 1ループ(フレーム)前のキーボード情報
	static char oldkeys[256];
	//ジョイパッド情報
	static XINPUT_STATE joypad;
	//一フレーム前のジョイパッド情報
	static XINPUT_STATE oldjoypad;

	static float JoyLeftDeadLine;	//最大32767.0f
	static float JoyRightDeadLine;	//最大32767.0f

public:
	static void Update();
	static bool GetKey(int f_KeyCode);
	static bool GetKeyTrigger(int f_KeyCode);

	static FLOAT2 GetJoyRightStick();				//右スティックの入力(方向と傾き度合)
	static float GetJoyRightStickAngle();			//右スティックが入力されている角度を取得(radian)
	static float GetJoyRightTrigger();				//右トリガーの傾き具合を0~1の値で取得
	static bool isJoyRightStickBottom();			//右スティックが倒されているか(DeadLine込み)
	static FLOAT2 GetJoyLeftStick();				//左スティックの入力(方向と傾き度合)
	static float GetJoyLeftStickAngle();			//左スティックが入力されている角度を取得(radian)
	static float GetJoyLeftTrigger();				//左トリガーの傾き具合を0~1の値で取得。
	static bool isJoyLeftStickBottom();			//左スティックが倒されているか(DeadLine込み)
	static bool isJoyBottom(int JoyBottom);			//ジョイパッドボタンの押下情報を取得。
	static bool isJoyBottomTrigger(int JoyBottom);	//ジョイパッドボタンのトリガー押下情報を取得。
	static bool isJoyBottomRelease(int JoyBottom);	//ジョイパッドボタンのリリース押下情報を取得。
};