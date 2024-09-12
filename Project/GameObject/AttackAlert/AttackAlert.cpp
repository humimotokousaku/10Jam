#include "AttackAlert.h"

void AttackAlert::Initialize() {
	postEffectManager_ = PostEffectManager::GetInstance();

	// UIのスプライトを作成
	alertSprite_[0].Initialize("Textures/UI", "guide_pad.png");
	alertSprite_[0].SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 4 });

	alertSprite_[1].Initialize("Textures/UI", "guide_pad.png");
	alertSprite_[1].SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 4 });
}

void AttackAlert::Update() {

}