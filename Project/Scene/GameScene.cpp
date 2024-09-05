#include "GameScene.h"
#include "GameManager.h"
#include "SceneTransition/SceneTransition.h"

void GameScene::Initialize() {
	sceneNum = GAME_SCENE;
	textureManager_ = TextureManager::GetInstance();
	modelManager_ = ModelManager::GetInstance();
	audio_ = Audio::GetInstance();

	// モデル読み込み
	/// テクスチャの読み込み
	textureManager_->LoadTexture("", "uvChecker.png");
	// テクスチャの番号を取得
	textureHandle_ = textureManager_->GetSrvIndex("", "uvChecker.png");

	modelManager_->LoadModel("Models/SampleBlock", "cube.obj");
	collisionManager_ = std::make_unique<CollisionManager>();

	// カメラ
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->worldTransform_.translate = Vector3(0.0f, 0.0f, -45.0f);
	// プレイヤー
	player_ = std::make_unique<Player>();
	player_->SetCollisionManager(collisionManager_.get());
	player_->Initialize(camera_.get());

	terrain_ = std::make_unique<Terrain>();
	terrain_->Initialize();
	terrain_->SetCamera(camera_.get());
	terrain_->SetModel(modelManager_->FindModel("Models/SampleBlock", "cube.obj"));
	collisionManager_->SetColliderList(terrain_.get());
	terrain_->SetPosition(Vector3(0.0f, -20.0f, 0.0f));

}

void GameScene::Update() {

	// カメラ更新
	ImGui::Begin("Camera");
	ImGui::DragFloat3("Position", &camera_->worldTransform_.translate.x, 0.01f);
	ImGui::End();
	camera_->Update();

	// プレイヤー
	player_->Update();
	player_->ImGuiDraw();

	terrain_->Update();
	terrain_->ImGuiDraw();

	// 当たり判定
	collisionManager_->CheckAllCollisions();

}

void GameScene::Draw() {
	player_->Draw();
	terrain_->Draw();
}

void GameScene::Finalize() {
	// ゲームパッドの振動を消す
	Input::GetInstance()->GamePadVibration(0, 0, 0);

	// 基底クラスの解放
	IScene::Finalize();
}