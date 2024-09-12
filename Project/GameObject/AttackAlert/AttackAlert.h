#pragma once
#include "Sprite.h"
#include "PostEffectManager.h"

class AttackAlert {
public:
	AttackAlert() = default;
	~AttackAlert() = default;

	void Initialize();
	void Update();

private:
	PostEffectManager* postEffectManager_;

private:
	// アラート画像
	std::array<Sprite, 2> alertSprite_;
};