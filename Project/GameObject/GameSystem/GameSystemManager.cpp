#include "GameSystemManager.h"

uint32_t GameSystemManager::sGameTimer = 0;

void GameSystemManager::Update()
{
	// ゲーム停止中
	if (isStop_) {
		return;
	}

	// フレームカウント
	frameCounter_++;
	// ゲーム内時間進む
	if (frameCounter_ >= 60) {
		sGameTimer++;
		frameCounter_ = 0;
	}

}
