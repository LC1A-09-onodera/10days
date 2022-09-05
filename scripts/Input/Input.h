#pragma once
#include "DxLib.h"

class Input
{
	// 最新のキーボード情報用
	static char keys[256];
	// 1ループ(フレーム)前のキーボード情報
	static char oldkeys[256];
public:
	static void Update();
	static bool GetKey(int f_KeyCode);
	static bool GetKeyTrigger(int f_KeyCode);
};