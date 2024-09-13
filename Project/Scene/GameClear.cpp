#include "GameClear.h"
#include "GameManager.h"
#include "SceneTransition/SceneTransition.h"

void GameClear::Initialize() {
	sceneNum = GAMECLEAR_SCENE;

	// UIのスプライトを作成
	guideUI_[0] = std::make_unique<Sprite>();
	guideUI_[0]->Initialize("Models/Skydome", "background.png");
	guideUI_[0]->SetSize(Vector2{ 1280,720 });
	guideUI_[0]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 2 });
	guideUI_[1] = std::make_unique<Sprite>();
	guideUI_[1]->Initialize("Models/Skydome", "background.png");
	guideUI_[1]->SetSize(Vector2{ 1280,720 });
	guideUI_[1]->SetPos(Vector2{ -(float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 2 });
	guideUI_[2] = std::make_unique<Sprite>();
	guideUI_[2]->Initialize("Textures/UI", "gameClear.png");
	guideUI_[2]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 4 });
	guideUI_[3] = std::make_unique<Sprite>();
	guideUI_[3]->Initialize("Textures/UI", "guide_pad_A.png");
	guideUI_[3]->SetSize(Vector2{ 64,64 });
	guideUI_[3]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 4 * 3 });
	guideUI_[4] = std::make_unique<Sprite>();
	guideUI_[4]->Initialize("Textures/UI", "guide_pad_A.png");
	guideUI_[4]->SetSize(Vector2{ 64,64 });
	guideUI_[4]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 4 * 3 });
	for (int i = 0; i < guideUI_.size(); i++) {
		PostEffectManager::GetInstance()->AddSpriteList(guideUI_[i].get());
	}

	// Aボタンの残像を出す
	buttonAnim_[0].SetAnimData(&guideUI_[3]->worldTransform_.scale, Vector3{ 1,1,1 }, Vector3{ 2,2,2 }, 60, "s", Easings::EaseOutExpo);
	buttonAnim_[1].SetAnimData(guideUI_[3]->GetColorP(), Vector4{ 1,1,1,0.6f }, Vector4{ 1,1,1,0 }, 60, "s1", Easings::EaseOutExpo);
	// ボタンの拡大と縮小
	buttonAnim_[2].SetAnimData(&guideUI_[2]->worldTransform_.scale, Vector3{ 1,1,1 }, Vector3{ 0.75f,0.75f,0.75f }, 5, "s", Easings::EaseInOutSine);
}

void GameClear::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE) || Input::GetInstance()->GamePadTrigger(XINPUT_GAMEPAD_A)) {
		for (int i = 0; i < buttonAnim_.size(); i++) {
			buttonAnim_[i].SetIsStart(true);
		}
	}

	if (buttonAnim_[1].GetIsEnd()) {
		SceneTransition::GetInstance()->Start();
		preSceneNum = GAMECLEAR_SCENE;
	}
	if (SceneTransition::GetInstance()->GetSceneTransitionSignal()) {
		sceneNum = TITLE_SCENE;
	}
	// ボタンのアニメーション更新
	for (int i = 0; i < buttonAnim_.size(); i++) {
		buttonAnim_[i].Update();
	}

	Vector2 pos = guideUI_[0]->GetPos();
	if (pos.x - 640.0f > 1280.0f) {
		guideUI_[0]->SetPos(Vector2{ guideUI_[1]->GetPos().x - 1280.0f + 0.1f, pos.y });
	}
	else {
		guideUI_[0]->SetPos(Vector2{ pos.x + 0.1f, pos.y });
	}
	pos = guideUI_[1]->GetPos();
	if (pos.x - 640.0f > 1280.0f) {
		guideUI_[1]->SetPos(Vector2{ guideUI_[0]->GetPos().x - 1280.0f + 0.1f, pos.y });
	}
	else {
		guideUI_[1]->SetPos(Vector2{ pos.x + 0.1f, pos.y });
	}

}

void GameClear::Draw() {

}

void GameClear::Finalize() {
	// 基底クラスの解放
	IScene::Finalize();
}