#include "GameTimer.h"

void GameTimer::Initialize() {
	textureManager_ = TextureManager::GetInstance();
	postEffectManager_ = PostEffectManager::GetInstance();

	for (int i = 0; i < numbersTexture_.size(); i++) {
		textureManager_->LoadTexture("Textures/UI", std::to_string(i) + ".png");
		numbersTexture_[i] = textureManager_->GetSrvIndex("Textures/UI", std::to_string(i) + ".png");
	}
	for (int i = 0; i < 3; i++) {
		numSprite_[i] = std::make_unique<Sprite>();
		numSprite_[i]->Initialize("Textures/UI", "0.png");
		numSprite_[i]->SetAnchorPoint(Vector2{ 0.5f, 0.5f });
		numSprite_[i]->SetPos(Vector2{ ((1280.0f / 2.0f) + 32.0f * i) - 32.0f, 32.0f });
		numSprite_[i]->SetSize(numSprite_[i]->GetSize() * 2.0f);
		postEffectManager_->AddSpriteList(numSprite_[i].get());

		eachTimer_[i] = 0;
	}

	frame_ = 0;
	timer_ = 997;
}

void GameTimer::Update() {
	for (int i = 0; i < 3; i++) {
		if (i == 0) {
			tempTimer_ = timer_;
			eachTimer_[i] = tempTimer_ / 100;
			tempTimer_ = tempTimer_ % 100;
		}
		if (i == 1) {
			eachTimer_[i] = tempTimer_ / 10;
			tempTimer_ = tempTimer_ % 10;
		}
		if (i == 2) {
			eachTimer_[i] = tempTimer_ / 1;
			tempTimer_ = tempTimer_ % 1;
		}

		// それぞれの桁に適した数値の画像を差し替える
		numSprite_[i]->SetTextureIndex(numbersTexture_[eachTimer_[i]]);
	}

	AddTimer();
}

void GameTimer::AddTimer() {
	if (timer_ >= 999) { return; }
	// 60フレーム経ったら1秒追加
	if (frame_ >= 60) {
		timer_++;
		frame_ = 0;
	}
	frame_++;
}
