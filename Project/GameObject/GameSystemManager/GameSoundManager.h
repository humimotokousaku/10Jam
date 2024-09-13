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
	void PlaySEAudio(std::string tag);
	void PlayBGMAudio(std::string tag);
	void StopAudio(std::string tag);
	void LoadAudio(std::string tag, std::string filePath);

public:
	static float sBGMVolume;
	static float sSEVolume;

private:
	Audio* audio_ = nullptr;
	std::unordered_map<std::string, uint32_t> soundContainer_;

};
