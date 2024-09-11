#include "Tutorial.h"

void Tutorial::Initialize() {
	// インスタンスを取得
	input_ = Input::GetInstance();
	sceneTransition_ = SceneTransition::GetInstance();
	postEffectManager_ = PostEffectManager::GetInstance();

#pragma region スプライト
	// UIのスプライトを作成
	guideUI_[0] = std::make_unique<Sprite>();
	guideUI_[0]->Initialize("Textures/UI", "guide_pad.png");
	guideUI_[0]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 4 + 32.0f });
	guideUI_[0]->SetSize(guideUI_[0]->GetSize() * 2);
	guideUI_[1] = std::make_unique<Sprite>();
	guideUI_[1]->Initialize("Textures/UI", "guide_move.png");
	guideUI_[1]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 4 });
	guideUI_[2] = std::make_unique<Sprite>();
	guideUI_[2]->Initialize("Textures/UI", "guide_gameStart.png");
	guideUI_[2]->SetPos(Vector2{ (float)WinApp::kClientWidth_ - (guideUI_[2]->GetSize().x / 2 + 32.0f),(float)WinApp::kClientHeight_ - 64.0f});
	guideUI_[2]->isActive_ = false;
	guideUI_[3] = std::make_unique<Sprite>();
	guideUI_[3]->Initialize("Textures/UI", "guide_pad_A.png");
	guideUI_[3]->SetSize(Vector2{ 32,32 });
	guideUI_[3]->SetPos(Vector2{ (float)WinApp::kClientWidth_ - (guideUI_[2]->GetSize().x + 48.0f),(float)WinApp::kClientHeight_ - 64.0f });
	guideUI_[3]->isActive_ = false;
	// "開始"
	guideUI_[4] = std::make_unique<Sprite>();
	guideUI_[4]->Initialize("Textures/UI", "guide_start.png");
	guideUI_[4]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 4 });
	guideUI_[4]->isActive_ = false;
	// "開始"の残像用
	guideUI_[5] = std::make_unique<Sprite>();
	guideUI_[5]->Initialize("Textures/UI", "guide_start.png");
	guideUI_[5]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 4 });
	guideUI_[5]->isActive_ = false;
	for (int i = 0; i < guideUI_.size(); i++) {
		// ポストエフェクトを使用しない
		postEffectManager_->AddSpriteList(guideUI_[i].get());
	}

	// カウントダウンの数字スプライトを作成
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
		postEffectManager_->AddSpriteList(countUI_[i].get());
	}
#pragma endregion

#pragma region アニメーション
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

	// "開始"スプライトの残像を出す
	startFontAnim_[0].SetAnimData(&guideUI_[5]->worldTransform_.scale, Vector3{ 1,1,1 }, Vector3{ 2,2,2 }, 50, "s", Easings::EaseOutExpo);
	startFontAnim_[1].SetAnimData(guideUI_[5]->GetColorP(), Vector4{ 1,1,1,0.6f }, Vector4{ 1,1,1,0 }, 50, "s1", Easings::EaseOutExpo);
	// "開始"スプライトの拡大と縮小
	startFontAnim_[2].SetAnimData(&guideUI_[4]->worldTransform_.scale, Vector3{ 1,1,1 }, Vector3{ 0.75f,0.75f,0.75f }, 5, "s", Easings::EaseInOutSine);
	// だんだん大きくなる
	startFontAnim_[2].SetAnimData(&guideUI_[4]->worldTransform_.scale, Vector3{ 1,1,1 }, Vector3{ 1.5f,1.5f,1.5f }, 40, "s", Easings::EaseOutExpo);
	startFontAnim_[2].SetAnimData(guideUI_[4]->GetColorP(), Vector4{ 1,1,1,1 }, Vector4{ 1,1,1,0 }, 10, "s1", Easings::EaseInExpo);
#pragma endregion

	isStart_ = true;
	isGameStart_ = false;
	isEnd_ = false;
}

void Tutorial::Update() {
	// チュートリアル終了しているなら処理しない
	if (isEnd_) { return; }

	// シーン遷移前のアニメーション開始
	if (input_->TriggerKey(DIK_SPACE) || input_->GamePadTrigger(XINPUT_GAMEPAD_A)) {
		// ゲーム開始できる状態なら遷移開始
		if (isGameStart_) {
			sceneTransition_->Start();
			// カウントダウンのフラグ
			isCountDown_ = true;
		}
	}

	// 3秒後にカウントダウン開始
	if (currentFrame_ <= 181 && currentFrame_ >= 180) {
		// ゲーム開始できる状態にする
		isGameStart_ = true;
		// ゲーム開始のUIを表示する
		for (int i = 2; i < guideUI_.size() - 2; i++) {
			guideUI_[i]->isActive_ = true;
		}
	}

	// 画面暗転時の処理
	if (sceneTransition_->GetSceneTransitionSignal()) {
		// コントローラのUIを非表示
		for (int i = 0; i < guideUI_.size(); i++) {
			guideUI_[i]->isActive_ = false;
		}
		// 3番の表示開始
		countUI_[0]->isActive_ = true;
		// 始めのアニメーションを開始
		buttonAnim_[0].SetIsStart(true);
		buttonAnim_[3].SetIsStart(true);
		// プレイヤーの座標設定
		player_->GenerateInitialize();
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
	for (int i = 0; i < startFontAnim_.size(); i++) {
		startFontAnim_[i].Update();
	}

	// 最後のアニメーションが終わったらチュートリアル終了
	if (buttonAnim_[buttonAnim_.size() - 1].GetIsEnd()) {
		isStart_ = false;
		isCountDown_ = false;
		// UIを表示しない
		for (int i = 0; i < countUI_.size(); i++) {
			countUI_[i]->isActive_ = false;
		}

		// "開始"を表示
		guideUI_[4]->isActive_ = true;
		guideUI_[5]->isActive_ = true;
		// アニメーション開始
		for (int i = 0; i < startFontAnim_.size(); i++) {
			startFontAnim_[i].SetIsStart(true);
		}
	}

	// 全てのアニメーションが終わっているなら処理しない
	if (startFontAnim_[2].GetIsEnd()) { 
		isEnd_ = true;
	}
	currentFrame_++;
}
