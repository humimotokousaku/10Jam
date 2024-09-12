#include "AttackAlert.h"

void AttackAlert::Initialize() {
	postEffectManager_ = PostEffectManager::GetInstance();

	// UIのスプライトを作成
	alertSprite_[0].Initialize("Textures/UI", "alertBelt.png");
	alertSprite_[0].SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 4 });
	alertSprite_[1].Initialize("Textures/UI", "guide_alert.png");
	alertSprite_[1].SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 4 });
	for (int i = 0; i < alertSprite_.size(); i++) {
		alertSprite_[i].isActive_ = false;
		postEffectManager_->AddSpriteList(&alertSprite_[i]);
	}

	// アニメーション
	alertAnim_[0].SetAnimData(alertSprite_[0].GetSizeP(), Vector2{1280.0f,0.0f}, Vector2{1280.0f,88.0f}, 5, "s", Easings::EaseOutExpo);
	alertAnim_[1].SetAnimData(alertSprite_[1].GetSizeP(), Vector2{370.0f,0.0f}, Vector2{370.0f,64.0f}, 5, "s", Easings::EaseOutExpo);

	isStart_ = false;
	currentFrame_ = kFinishAlert;
}

void AttackAlert::Update() {
	if (!isStart_) { return; }

	// アラート中はスプライトを常に表示
	for (int i = 0; i < alertSprite_.size(); i++) {
		alertSprite_[i].isActive_ = true;
	}
	// アニメーションの更新
	for (int i = 0; i < alertAnim_.size(); i++) {
		alertAnim_[i].Update();
	}

	// uvTransformを動かす
	alertSprite_[0].uvTransform_.translate.x += 0.05f;

	// 3秒経過したら終了
	if (currentFrame_ <= 0) {
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

	currentFrame_--;
}