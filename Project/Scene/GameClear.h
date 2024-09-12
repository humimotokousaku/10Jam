#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "Animation.h"

class GameManager;
class GameClear : public IScene
{
public:
	~GameClear()override = default;

	// 初期化
	void Initialize()override;

	// 更新処理
	void Update()override;

	// 描画
	void Draw()override;

	void Finalize()override;
private:
	// UI
	std::array<std::unique_ptr<Sprite>, 4> guideUI_;

	// ボタンのアニメーション
	std::array<Animation, 4> buttonAnim_;
};

