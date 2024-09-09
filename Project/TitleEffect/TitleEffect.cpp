#include "TitleEffect.h"
#include "SceneTransition/SceneTransition.h"

void TitleEffect::Initialize() {
	// インスタンスを取得
	input_ = Input::GetInstance();
	modelManager_ = ModelManager::GetInstance();
	textureManager_ = TextureManager::GetInstance();

#pragma region テクスチャとモデルの読み込み
	// テクスチャの読み込み
	textureManager_->LoadTexture("", "uvChecker.png");
	// テクスチャの番号を取得
	textureHandle_ = textureManager_->GetSrvIndex("", "uvChecker.png");
	// 使用するモデルの読み込み
	modelManager_->LoadModel("Models/SampleBlock", "cube.obj");
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
	// だるまを生成
	player_->GetPartManager().AddDaruma(PlayerContext::DarumaPattern::kDefault);
	player_->SetPushPower(Vector3{ 1.8f, 0.0f, 0.0f });

	// 地面
	terrain_ = std::make_unique<Terrain>();
	terrain_->Initialize();
	terrain_->SetCamera(camera_.get());
	terrain_->SetModel(modelManager_->FindModel("Models/SampleBlock", "cube.obj"));
	collisionManager_->SetColliderList(terrain_.get());
	terrain_->SetPosition(Vector3(0.0f, -20.0f, 0.0f));
}

void TitleEffect::Update() {
	// シーン遷移前のアニメーション開始
	if (input_->TriggerKey(DIK_SPACE) || input_->GamePadTrigger(XINPUT_GAMEPAD_A)) {
		SceneTransition::GetInstance()->Start();
	}

	// プレイヤー
	player_->ImGuiDraw();
	player_->Update();

	// 地面
	terrain_->Update();
	terrain_->ImGuiDraw();

	// カメラ
	camera_->Update();

	// 衝突マネージャー
	collisionManager_->CheckAllCollisions();
}

void TitleEffect::Draw() {
	// 自機
	player_->Draw();
	// 地面
	terrain_->Draw(textureHandle_);
}