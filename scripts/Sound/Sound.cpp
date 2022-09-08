#include "Sound.h"
#include "DxLib.h"

int SoundManager::shotHitSound;

void SoundManager::LoadFile()
{
	shotHitSound = LoadSoundMem("Resources/Sound/Hit1.wav");
}