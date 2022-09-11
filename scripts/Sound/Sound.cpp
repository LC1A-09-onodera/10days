#include "Sound.h"
#include "DxLib.h"

int SoundManager::shotHitSound;
int SoundManager::shotBullet;
int SoundManager::refrect;
int SoundManager::addScore;
void SoundManager::LoadFile()
{
	shotHitSound = LoadSoundMem("Resources/Sound/hit.wav");
	shotBullet = LoadSoundMem("Resources/Sound/shot.wav");
	refrect = LoadSoundMem("Resources/Sound/reflect.wav");
	addScore = LoadSoundMem("Resources/Sound/increase_score.wav");
}