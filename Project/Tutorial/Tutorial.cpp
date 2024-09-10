#include "Tutorial.h"
#include "SceneTransition/SceneTransition.h"

void Tutorial::Initialize(CollisionManager* collisionManager, Camera* camera, Player* player, Enemy* enemy, Terrain* terrain, GameSystemManager* gameSystemManager) {
	collisionManager_ = collisionManager;
	camera_ = camera;
	player_ = player;
	enemy_ = enemy;
	terrain_ = terrain;
	gameSystemManager_ = gameSystemManager;

	// UIのスプライトを作成
	guideUI_[0] = std::make_unique<Sprite>();
	guideUI_[0]->Initialize("Textures/UI", "guide_pad.png");
	guideUI_[0]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 4 + 32.0f });
	guideUI_[0]->SetSize(guideUI_[0]->GetSize() * 2);
	guideUI_[1] = std::make_unique<Sprite>();
	guideUI_[1]->Initialize("Textures/UI", "guide_move.png");
	guideUI_[1]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 4 });
	for (int i = 0; i < guideUI_.size(); i++) {
		// ポストエフェクトを使用しない
		PostEffectManager::GetInstance()->AddSpriteList(guideUI_[i].get());
	}

	// UIのスプライトを作成
	countUI_[0] = std::make_unique<Sprite>();
	countUI_[0]->Initialize("Textures/UI", "3.png");
	countUI_[0]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 2 });
	countUI_[0]->SetSize(countUI_[0]->GetSize() * 2);
	countUI_[1] = std::make_unique<Sprite>();
	countUI_[1]->Initialize("Textures/UI", "2.png");
	countUI_[1]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 2 });
	countUI_[1]->SetSize(countUI_[1]->GetSize() * 2);
	countUI_[2] = std::make_unique<Sprite>();
	countUI_[2]->Initialize("Textures/UI", "1.png");
	countUI_[2]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 2 });
	countUI_[2]->SetSize(countUI_[2]->GetSize() * 2);

	for (int i = 0; i < countUI_.size(); i++) {
		// UIを表示しない
		countUI_[i]->isActive_ = false;
		// ポストエフェクトを使用しない
		PostEffectManager::GetInstance()->AddSpriteList(countUI_[i].get());
	}

	// 数字を拡大縮小
	buttonAnim_[0].SetAnimData(countUI_[0]->GetSizeP(), Vector2{ 0.0f,0.0f }, Vector2{ 256.0f,256.0f }, 60, "s", Easings::EaseOutExpo);
	buttonAnim_[0].SetAnimData(countUI_[0]->GetSizeP(), Vector2{ 256.0f,256.0f }, Vector2{ 0.0f,0.0f }, 5, "s", Easings::EaseOutExpo);
	buttonAnim_[1].SetAnimData(countUI_[1]->GetSizeP(), Vector2{ 0.0f,0.0f }, Vector2{ 256.0f,256.0f }, 60, "s", Easings::EaseOutExpo);
	buttonAnim_[1].SetAnimData(countUI_[1]->GetSizeP(), Vector2{ 256.0f,256.0f }, Vector2{ 0.0f,0.0f }, 5, "s", Easings::EaseOutExpo);
	buttonAnim_[2].SetAnimData(countUI_[2]->GetSizeP(), Vector2{ 0.0f,0.0f }, Vector2{ 256.0f,256.0f }, 60, "s", Easings::EaseOutExpo);
	buttonAnim_[2].SetAnimData(countUI_[2]->GetSizeP(), Vector2{ 256.0f,256.0f }, Vector2{ 0.0f,0.0f }, 5, "s", Easings::EaseOutExpo);
	// 回転
	buttonAnim_[3].SetAnimData(&countUI_[0]->worldTransform_.rotate, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 0.0f,0.0f,(float)std::numbers::pi * 2 }, 60, "s", Easings::EaseOutExpo);
	buttonAnim_[4].SetAnimData(&countUI_[1]->worldTransform_.rotate, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 0.0f,0.0f,(float)std::numbers::pi * 2 }, 60, "s", Easings::EaseOutExpo);
	buttonAnim_[5].SetAnimData(&countUI_[2]->worldTransform_.rotate, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 0.0f,0.0f,(float)std::numbers::pi * 2 }, 60, "s", Easings::EaseOutExpo);

	isStart_ = true;
}

void Tutorial::Update() {
	// 3秒後にカウントダウン開始
	if (currentFrame_ <= 241 && currentFrame_ >= 240) {
		SceneTransition::GetInstance()->Start();
	}
	if (SceneTransition::GetInstance()->GetSceneTransitionSignal()) {
		// コントローラのUIを非表示
		for (int i = 0; i < guideUI_.size(); i++) {
			guideUI_[i]->isActive_ = false;
		}
		// 3番の表示開始
		countUI_[0]->isActive_ = true;
		// 始めのアニメーションを開始
		buttonAnim_[0].SetIsStart(true);
		buttonAnim_[3].SetIsStart(true);
	}
	// ひとつ前のアニメーションが終了したらアニメーション開始
	for (int i = 0; i < buttonAnim_.size() / 2 - 1; i++) {
		// 終了を検出
		if (buttonAnim_[i].GetIsEnd()) {
			countUI_[i + 1]->isActive_ = true;
			buttonAnim_[i + 1].SetIsStart(true);
			buttonAnim_[i + 4].SetIsStart(true);
		}
	}

	// アニメーションの更新
	for (int i = 0; i < buttonAnim_.size(); i++) {
		buttonAnim_[i].Update();
	}

	// 最後のアニメーションが終わったらチュートリアル終了
	if (buttonAnim_[buttonAnim_.size() - 1].GetIsEnd()) {
		isStart_ = false;
		// UIを表示しない
		for (int i = 0; i < countUI_.size(); i++) {
			countUI_[i]->isActive_ = false;
		}
	}
	currentFrame_++;
}
