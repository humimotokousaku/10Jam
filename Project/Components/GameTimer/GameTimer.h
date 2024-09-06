#pragma once
#include "Sprite.h"
#include "PostEffectManager.h"
#include <array>
#include <memory>

class GameTimer {
public:
	GameTimer() = default;
	~GameTimer() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 秒数を増加
	/// </summary>
	void AddTimer();

	void SetDrawTime(int time) { timer_ = time; }

private:
	TextureManager* textureManager_;
	PostEffectManager* postEffectManager_;

	std::array<uint32_t, 10> numbersTexture_;
	// スコアのスプライト(サイズが桁数)
	std::array<std::unique_ptr<Sprite>, 3> numSprite_;

	// フレーム
	int frame_;
	// 表示する時間[秒]
	int timer_;
	int tempTimer_;
	std::array<int, 3> eachTimer_;
};