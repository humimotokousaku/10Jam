#pragma once
#include <string>
#include "Audio.h"

class GameSoundManager
{
public:
	~GameSoundManager() = default;
	void Initialize();
	void PlayAudio(std::string tag);

private:
	Audio* audio_ = nullptr;


};
