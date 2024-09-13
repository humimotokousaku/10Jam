#include "GameSoundManager.h"

GameSoundManager::GameSoundManager()
{
	audio_ = Audio::GetInstance();
}

void GameSoundManager::PlayAudio(std::string tag)
{
	if (soundContainer_.find(tag) == soundContainer_.end()) {
		return;
	}
	// 再生
	audio_->SoundPlayWave(soundContainer_[tag]);

}

void GameSoundManager::LoadAudio(std::string tag, std::string filePath)
{
	if (soundContainer_.find(tag) != soundContainer_.end()) {
		return;
	}
	uint32_t sound = audio_->SoundLoadWave(filePath.c_str());
	soundContainer_.emplace(tag, sound);
}
