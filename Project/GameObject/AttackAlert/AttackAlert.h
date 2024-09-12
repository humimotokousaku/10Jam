#pragma once
#include "Sprite.h"
#include "Animation.h"
#include "PostEffectManager.h"

/// <summary>
/// 強い攻撃を警告するクラス
/// </summary>
class AttackAlert {
public:
	AttackAlert() = default;
	~AttackAlert() = default;

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

	// アラート開始
	void Start() { 
		isStart_ = true; 
		for (int i = 0; i < alertAnim_.size(); i++) {
			alertAnim_[i].SetIsStart(true);
		}
	}
	// アラートを強制終了
	void End() { 
		isStart_ = false;
		currentFrame_ = kFinishAlert;
		alertSprite_[0].uvTransform_.translate = { 0,0,0 };
		for (int i = 0; i < alertSprite_.size(); i++) {
			alertSprite_[i].isActive_ = false;
		}
		// アニメーションを終了
		for (int i = 0; i < alertAnim_.size(); i++) {
			alertAnim_[i].SetIsStart(false);
		}
	}

private:// エンジン機能
	PostEffectManager* postEffectManager_;

private:// 定数
	// アラート表示時間(3秒)
	const int kFinishAlert = 180;

private:
	// アラート画像
	std::array<Sprite, 2> alertSprite_;

	// アニメーション
	std::array<Animation, 2> alertAnim_;

	// アラート起動
	bool isStart_;

	// 経過時間
	int currentFrame_;
};