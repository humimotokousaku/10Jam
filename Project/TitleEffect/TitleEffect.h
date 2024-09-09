#pragma once
#include "../GameObjectLists.h"

class TitleEffect {
public:
	~TitleEffect() = default;

	// 初期化
	void Initialize();
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

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
	// 地面
	std::unique_ptr<Terrain> terrain_;
};