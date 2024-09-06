#pragma once
#include <stdint.h>

class GameSystemManager
{
public:
	// 初期化
	void Initialize();
	// ゲーム時の処理
	void Update();

public:
	// 経過時間の取得
	uint32_t GetElapsedTime() { return elapsedTime_; }

private: // 時間関係
	// ゲームの経過時間
	uint32_t elapsedTime_;
	// ゲームのフレームカウント（経過時間用）
	uint32_t frameCounter_ = 0;
	// ゲームを止める処理
	bool isStop_ = false;
	// ゲームのポーズ
	bool isPause_ = false;
private: // ゲーム全体の管理
	// ゲーム処理の終了フラグ
	bool isGameEnd_;

};
