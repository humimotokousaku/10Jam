#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "ModelManager.h"
#include "TextureManager.h"

#include "TitleEffect/TitleEffect.h"

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

private:// エンジン機能
	// 入力
	Input* input_;
	// モデルの読み込み
	ModelManager* modelManager_;
	// テクスチャの読み込み
	TextureManager* textureManager_;

private:
	// タイトルの演出
	TitleEffect titleEffect_;
};