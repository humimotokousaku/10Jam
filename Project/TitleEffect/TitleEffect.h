#pragma once
#include "Animation.h"
#include "../GameObjectLists.h"
#include "../GameSystemManager/GameSystemManager.h"

class TitleEffect {
public:
	~TitleEffect() = default;

	// 初期化
	void Initialize();
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

private:// 定数
	// だるまの体を押し出す回数
	const int kPushCount = 3;
	// 演出時間
	const int kAllEffectFrame = 120;

private:
	// 入力
	Input* input_;
	// モデルの読み込み
	ModelManager* modelManager_;
	// テクスチャの読み込み
	TextureManager* textureManager_;

	// テクスチャの番号
	uint32_t textureHandle_;

	// カメラ
	std::unique_ptr<Camera> camera_;
	// 衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager_;

	// 自機
	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;
	// 地面
	std::unique_ptr<Terrain> terrain_;
	// UI
	std::array<std::unique_ptr<Sprite>, 3> guideUI_;

	// ボタンのアニメーション
	std::array<Animation, 3> buttonAnim_;

	// 演出開始
	bool isStart_;
	// 演出開始時間
	int currentFrame_;
	// だるまを押し出す回数
	int count_;

	int delayCurrentFrame_ = 0;

};