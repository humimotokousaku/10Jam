#pragma once
#include <string>
#include <unordered_map>
#include "Audio.h"

class GameSoundManager
{
public:
	static GameSoundManager* GetInstance() {
		static GameSoundManager instance;
		return &instance;
	}
	GameSoundManager();
	~GameSoundManager() = default;
	void PlayAudio(std::string tag);
	void LoadAudio(std::string tag, std::string filePath);

private:
	Audio* audio_ = nullptr;

	std::unordered_map<std::string, uint32_t> soundContainer_;

};
