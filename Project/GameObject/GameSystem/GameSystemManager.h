#pragma once
#include <stdint.h>

class GameSystemManager
{
public:
	// ゲーム内時間
	static uint32_t sGameTimer;
	// ゲーム時の処理
	void Update();

private:
	// ゲーム中のフレームカウント
	uint32_t frameCounter_ = 0;
	// ゲームを止める処理
	bool isStop_ = false;
	// ゲームのポーズ
	bool isPause_ = false;
};
