#include "TitleEffect.h"
#include "SceneTransition/SceneTransition.h"
#include "ImGuiManager.h"

void TitleEffect::Initialize() {
	// インスタンスを取得
	input_ = Input::GetInstance();
	modelManager_ = ModelManager::GetInstance();
	textureManager_ = TextureManager::GetInstance();

#pragma region テクスチャとモデルの読み込み
	// テクスチャの読み込み
	textureManager_->LoadTexture("", "uvChecker.png");
	textureManager_->LoadTexture("Models/DarumaHead", "DarumaHeadWeak.png");
	textureManager_->LoadTexture("Models/DarumaBody", "DarumaBodyBlue.png");
	textureManager_->LoadTexture("Models/DarumaBody", "DarumaBodyYellow.png");
	textureManager_->LoadTexture("Models/DarumaBody", "DarumaBodyGreen.png");
	textureManager_->LoadTexture("Models/DarumaBody", "DarumaBodyPurple.png");
	// テクスチャの番号を取得
	textureHandle_ = textureManager_->GetSrvIndex("", "uvChecker.png");
	// 使用するモデルの読み込み
	modelManager_->LoadModel("Models/SampleBlock", "cube.obj");
	modelManager_->LoadModel("Models/DarumaHead", "DarumaHead.obj");
	modelManager_->LoadModel("Models/DarumaBody", "DarumaBody.obj");
	modelManager_->LoadModel("Models/Floor", "Floor.obj");
#pragma endregion

	// カメラの作成
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->worldTransform_.translate = { 0.7f,-14.0f,-18.0f };
	camera_->worldTransform_.rotate = { -0.2f,0.33f,0.0f };

	// 衝突マネージャーを生成
	collisionManager_ = std::make_unique<CollisionManager>();

	// プレイヤー
	player_ = std::make_unique<Player>();
	player_->SetCollisionManager(collisionManager_.get());
	player_->Initialize(camera_.get());
	player_->GetPartManager()->AddDaruma(PlayerContext::DarumaPattern::kL2M2H);
	player_->GetPartManager()->AddHead(Vector3{ 0.5f,40,0 });
	// エネミー
	enemy_ = std::make_unique<Enemy>();

	// 地面
	terrain_ = std::make_unique<Terrain>();
	terrain_->Initialize();
	terrain_->SetCamera(camera_.get());
	terrain_->SetModel(modelManager_->FindModel("Models/Floor", "Floor.obj"));
	collisionManager_->SetColliderList(terrain_.get());
	terrain_->SetPosition(Vector3(0.0f, -20.0f, 0.0f));

	// UIのスプライトを作成
	guideUI_[0] = std::make_unique<Sprite>();
	guideUI_[0]->Initialize("Textures/UI", "titleName.png");
	guideUI_[0]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 4 });
	guideUI_[1] = std::make_unique<Sprite>();
	guideUI_[1]->Initialize("Textures/UI", "guide_pad_A.png");
	guideUI_[1]->SetSize(Vector2{ 64,64 });
	guideUI_[1]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 4 * 3 });
	guideUI_[2] = std::make_unique<Sprite>();
	guideUI_[2]->Initialize("Textures/UI", "guide_pad_A.png");
	guideUI_[2]->SetSize(Vector2{ 64,64 });
	guideUI_[2]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 4 * 3 });
	for (int i = 0; i < guideUI_.size(); i++) {
		PostEffectManager::GetInstance()->AddSpriteList(guideUI_[i].get());
	}

	// Aボタンの残像を出す
	buttonAnim_[0].SetAnimData(&guideUI_[2]->worldTransform_.scale, Vector3{ 1,1,1 }, Vector3{ 2,2,2 }, 60, "s", Easings::EaseOutExpo);
	buttonAnim_[1].SetAnimData(guideUI_[2]->GetColorP(), Vector4{ 1,1,1,0.6f }, Vector4{ 1,1,1,0 }, 60, "s1", Easings::EaseOutExpo);
	// ボタンの拡大と縮小
	buttonAnim_[2].SetAnimData(&guideUI_[1]->worldTransform_.scale, Vector3{ 1,1,1 }, Vector3{ 0.75f,0.75f,0.75f }, 5, "s", Easings::EaseInOutSine);

	isStart_ = false;
	currentFrame_ = kAllEffectFrame;
	count_ = kPushCount;
	delayCurrentFrame_ = 0;
}

void TitleEffect::Update() {
#ifdef _DEBUG
	ImGui::Begin("TitleEffect");
	ImGui::InputInt("Delay", &delayCurrentFrame_);
	ImGui::End();
#endif // _DEBUG
	if (delayCurrentFrame_ < 90) {
		delayCurrentFrame_++;
	}
	// シーン遷移前のアニメーション開始
	if ((input_->TriggerKey(DIK_SPACE) || input_->GamePadTrigger(XINPUT_GAMEPAD_A)) && (delayCurrentFrame_ > 45)) {
  		isStart_ = true;
		// Aボタンアニメーション開始
		for (int i = 0; i < buttonAnim_.size(); i++) {
			buttonAnim_[i].SetIsStart(true);
		}
	}
	// Aボタンアニメーションの更新
	for (int i = 0; i < buttonAnim_.size(); i++) {
		buttonAnim_[i].Update();
	}

	// プレイヤー
	player_->Update(true);

	// 地面
	terrain_->Update();

	// カメラ
	camera_->Update();

	// 衝突マネージャー
	collisionManager_->CheckAllCollisions();

	// 押し返し
	player_->SolveConstraints();

	// 演出開始
	if (isStart_) {
		if (count_ > 0) {
			// 三回だるまの体を落とす
			if (currentFrame_ % 30 == 0) {
				//gameSystemManager_->Action(5.0f);
				player_->GetReactionManager()->PushAction({ 1.0f,0.0f,0.0f }, 5.0f);
				count_--;
			}
		}
		// 演出時間が過ぎたらシーン遷移開始
		if (currentFrame_ <= 0) {
			isStart_ = false;
			currentFrame_ = kAllEffectFrame;
			count_ = kPushCount;
			SceneTransition::GetInstance()->Start();
		}
		currentFrame_--;
	}
}

void TitleEffect::Draw() {
	// 自機
	player_->Draw();
	// 地面
	terrain_->Draw();
}