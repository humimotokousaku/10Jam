#include "GameScene.h"
#include "GameManager.h"
#include "SceneTransition/SceneTransition.h"

void GameScene::Initialize() {
	sceneNum = GAME_SCENE;
	textureManager_ = TextureManager::GetInstance();
	modelManager_ = ModelManager::GetInstance();
	audio_ = Audio::GetInstance();

	// モデル読み込み
	modelManager_->LoadModel("", "cube.obj");
	collisionManager_ = std::make_unique<CollisionManager>();

	// カメラ
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	// プレイヤー
	player_ = std::make_unique<Player>();
	player_->SetCollisionManager(collisionManager_.get());
	player_->Initialize(camera_.get());

}

void GameScene::Update() {

	// カメラ更新
	camera_->Update();
	// プレイヤー
	player_->Update();


	// 当たり判定
	collisionManager_->CheckAllCollisions();

}

void GameScene::Draw() {
	player_->Draw();
}

void GameScene::Finalize() {
	// ゲームパッドの振動を消す
	Input::GetInstance()->GamePadVibration(0, 0, 0);

	// 基底クラスの解放
	IScene::Finalize();
}