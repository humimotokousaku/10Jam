#pragma once
#include <stdint.h>
#include <memory>
#include "../AttackDirection/AttackDirection.h"

class Player;
class Enemy;

class GameSystemManager
{
private: // 時間関係
	struct GameTime {
		// ゲームの経過時間
		uint32_t elapsed;
		// ゲームのフレームカウント（経過時間用）
		uint32_t frameCount = 0;
		// ゲームを止める処理
		bool isStop = false;
		// ゲームのポーズ
		bool isPause = false;
		// 更新
		void Update() {
			// フレームカウント
			frameCount++;
			// ゲーム内時間進む
			if (frameCount >= 60) {
				elapsed++;
				frameCount = 0;
			}
		}
	};
	// タイマー
	GameTime timer_;

public:
	// 初期化
	void Initialize(Player* player, Enemy* enemy);
	// ゲーム時の処理
	void Update();
	void ImGuiDraw();
	void Draw();
	// 行動
	void Action(float power);

	struct CountParam {
		uint32_t current;
		uint32_t max;
	};
	struct EnemyActionTable {
		// 行動用
		CountParam action;
		// 間隔用
		CountParam coolTime;
	};

	EnemyActionTable actionTime;

	bool isGameStop_ = false;

public:
	// 経過時間の取得
	uint32_t GetElapsedTime() { return timer_.elapsed; }

private: // ゲームの管理
	// ゲーム処理の終了フラグ
	bool isGameEnd_;
	// ポインタ
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;

	Vector3 actionDirect_{ 1.0f,0.0f,0.0f };
	float actionPower_ = 0.15f;
	// 攻撃方向の表示
	std::unique_ptr<AttackDirection> attackDirection_;

};
