#pragma once
#include "IScene.h"

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
	Input* input_;
};