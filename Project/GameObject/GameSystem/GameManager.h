#pragma once
#include <stdint.h>

class GameManager
{
public:
	// ゲーム内時間
	static uint32_t sGameTimer;
	// 
	uint32_t frameCounter_;
	// ゲームを止める処理
	bool isStop_;

	// ゲーム時の処理
	void Update();
};
