#include "Sound.h"
#include "DxLib.h"

int SoundManager::shotHitSound;
int SoundManager::shotBullet;
int SoundManager::refrect;
int SoundManager::addScore;
int SoundManager::reload;
int SoundManager::BGMVol = 255;
int SoundManager::BGM;
int SoundManager::BombShot;
int SoundManager::weponChange;
int SoundManager::select;

int SoundManager::damage;
int SoundManager::transe;

void SoundManager::LoadFile()
{
	shotHitSound = LoadSoundMem("Resources/Sound/hit.wav");
	shotBullet = LoadSoundMem("Resources/Sound/shot.wav");
	refrect = LoadSoundMem("Resources/Sound/reflect.wav");
	addScore = LoadSoundMem("Resources/Sound/increase_score.wav");
	reload = LoadSoundMem("Resources/Sound/reload.wav");
	BGM = LoadSoundMem("Resources/Sound/bgm.wav");
	BombShot = LoadSoundMem("Resources/Sound/bomb.wav");
	weponChange = LoadSoundMem("Resources/Sound/change.wav");
	select = LoadSoundMem("Resources/Sound/switch.wav");
	damage = LoadSoundMem("Resources/Sound/damage.wav");
	transe = LoadSoundMem("Resources/Sound/transverse.wav");
}