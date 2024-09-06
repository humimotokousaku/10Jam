#include "GameSystemManager.h"

void GameSystemManager::Initialize()
{
	elapsedTime_ = 0;
	frameCounter_ = 0;
}

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
		elapsedTime_++;
		frameCounter_ = 0;
	}

}
