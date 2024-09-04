#pragma once
#include "IScene.h"

// モデルの読み込みに使用
#include "ModelManager.h"

// テクスチャの読み込みに使用
#include "TextureManager.h"

// テスト用のブロック
#include "SampleObject/SampleObject.h"

// カメラ
#include "Camera.h"

class GameManager;

class TitleScene : public IScene
{
public:
	~TitleScene()override = default;

	// 初期化
	void Initialize() override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Draw() override;
	// 解放処理
	void Finalize()override;

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

	// テスト用ブロック
	std::array<std::unique_ptr<SampleObject>, 2> sampleObject_;
};