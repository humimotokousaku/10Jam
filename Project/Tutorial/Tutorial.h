#pragma once
#include "Sprite.h"
#include "Animation.h"
#include "../GameObjectLists.h"
#include "../GameSystemManager/GameSystemManager.h"

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

private:
	// UI
	std::array<std::unique_ptr<Sprite>, 2> guideUI_;

	// カウントダウンの数字
	std::array<std::unique_ptr<Sprite>, 3> countUI_;
	// カウントダウンの数字アニメーション
	std::array<Animation, 6> buttonAnim_;

	// チュートリアル開始
	bool isStart_;

	int currentFrame_;
};