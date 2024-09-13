#include "GameSoundManager.h"
#include "GlobalVariables.h"

float GameSoundManager::sBGMVolume = 0.5f;
float GameSoundManager::sSEVolume = 0.5f;

GameSoundManager::GameSoundManager()
{
	audio_ = Audio::GetInstance();
	GlobalVariables* global = GlobalVariables::GetInstance();
	global->CreateGroup("Sound");
	global->AddItem("Sound", "BGM", sBGMVolume);
	global->AddItem("Sound", "SE", sSEVolume);
}

void GameSoundManager::PlaySEAudio(std::string tag)
{
	if (soundContainer_.find(tag) == soundContainer_.end()) {
		return;
	}
	// 音量
	sBGMVolume = GlobalVariables::GetInstance()->GetFloatValue("Sound", "SE");
	// 再生
	audio_->SoundPlayWave(soundContainer_[tag], false, sSEVolume);
}

void GameSoundManager::PlayBGMAudio(std::string tag)
{
	if (soundContainer_.find(tag) == soundContainer_.end()) {
		return;
	}
	// 音量
	sBGMVolume = GlobalVariables::GetInstance()->GetFloatValue("Sound", "BGM");
	// 再生
	audio_->SoundPlayWave(soundContainer_[tag], true, sBGMVolume);
}

void GameSoundManager::StopAudio(std::string tag)
{
	if (soundContainer_.find(tag) == soundContainer_.end()) {
		return;
	}
	// 再生
	audio_->SoundStop(soundContainer_[tag]);
}

void GameSoundManager::LoadAudio(std::string tag, std::string filePath)
{
	soundContainer_.emplace(tag, audio_->SoundLoadWave(filePath.c_str()));
}
