#pragma once
#include <stdint.h>
#include <memory>
#include "../AttackDirection/AttackDirection.h"
#include "../AttackAlert/AttackAlert.h"
#include "GameTimer/GameTimer.h"
#include "ActionManager.h"

class Player;
class Enemy;

class GameSystemManager
{
public:
	static float sDeltaTime;
	// クリア時の数
	static int sClearPartCount;
public:
	// 初期化
	void Initialize(Player* player, Enemy* enemy);
	// ゲーム時の処理
	void Update(bool isTutorial);
	// ImGui
	void ImGuiDraw();
	// 描画
	void Draw();
	// 行動
	void Action(const Vector3& direct, float power);
private:
	// CSVのやつ
	void CSVActionControll();
	int alertSE_;
public:
	// 経過時間の取得
	uint32_t GetElapsedTime() { return timer_.elapsed; }
	bool IsGameEnd() { return isGameEnd_; }
	bool IsGameClear() { return isGameClear_; }
	bool IsGameOver() { return isGameOver_; }
	bool IsGameStop() { return isGameStop_; }
private: // ゲームの管理
	// ゲーム処理の終了フラグ
	bool isGameEnd_ = false;
	// 成功
	bool isGameClear_ = false;
	// 失敗
	bool isGameOver_ = false;
	// 停止
	bool isGameStop_ = false;
private: // システムクラス
	// 攻撃方向の表示
	std::unique_ptr<AttackDirection> attackDirection_;
	// 行動マネージャー
	std::unique_ptr<ActionManager> actionManager_;
	// 警告
	std::unique_ptr<AttackAlert> attackAlert_;

private:
	// ポインタ
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	// 押し出しの方向
	Vector3 actionDirect_{ 1.0f,0.0f,0.0f };
	// 押し出しの力
	float actionPower_ = 0.15f;
	// 現在のインデックス
	int32_t actionNow_ = 0;
	// ゲームのタイム
	GameTimer gameTimer_;
private: // 時間関係
	struct GameFrameTimer {
		// ゲームの経過時間
		int32_t elapsed = 0;
		// ゲームのフレームカウント（経過時間用）
		uint32_t frameCount = 0;
		// ゲームを止める処理
		bool isStop = false;
		// ゲームのポーズ
		bool isPause = false;
		// 更新
		void Update(int32_t value) {
			// フレームカウント
			frameCount++;
			// ゲーム内時間進む
			if (frameCount >= 60) {
				elapsed += value;
				frameCount = 0;
			}
			if (elapsed < 0) {
				elapsed = 0;
			}
		}
		bool isAction = false;
		// 値の初期化
		void Initialize() { *this = GameFrameTimer(); }
	};
	// タイマー
	GameFrameTimer timer_;
};
