#pragma once
#include "Sprite.h"
#include "Animation.h"
#include "../GameObjectLists.h"
#include "../GameSystemManager/GameSystemManager.h"
#include "SceneTransition/SceneTransition.h"
#include "PostEffectManager.h"

class Tutorial {
public:
	Tutorial() = default;
	~Tutorial() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	///
	/// User Method
	/// 

	/// <summary>
	/// チュートリアル開始
	/// </summary>
	void Start() { isStart_ = true; }

#pragma region Getter
	// チュートリアルが開始しているか
	bool GetIsStart() { return isStart_; }
#pragma endregion

private:// エンジン機能
	// 入力
	Input* input_;
	// シーン遷移
	SceneTransition* sceneTransition_;
	// ポストエフェクト
	PostEffectManager* postEffectManager_;

private:
	// 文字やコントローラなどのUI
	std::array<std::unique_ptr<Sprite>, 6> guideUI_;
	// カウントダウンの数字
	std::array<std::unique_ptr<Sprite>, 3> countUI_;

	// カウントダウンの数字アニメーション
	std::array<Animation, 6> buttonAnim_;
	// "開始"のアニメーション
	std::array<Animation, 3> startFontAnim_;

	// チュートリアル開始
	bool isStart_;
	// ゲーム開始可能
	bool isGameStart_;
	// チュートリアル終了
	bool isEnd_;

	// 経過時間
	int currentFrame_;
};