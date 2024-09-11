#include "GameScene.h"
#include "GameManager.h"
#include "SceneTransition/SceneTransition.h"
#include "GlobalVariables.h"

void GameScene::Initialize() {
	sceneNum = GAME_SCENE;
	textureManager_ = TextureManager::GetInstance();
	modelManager_ = ModelManager::GetInstance();
	audio_ = Audio::GetInstance();

	/// テクスチャの読み込み
	textureManager_->LoadTexture("", "uvChecker.png");
	textureManager_->LoadTexture("DefaultTexture", "white.png");
	// テクスチャの番号を取得
	textureHandle_ = textureManager_->GetSrvIndex("DefaultTexture", "white.png");
	
	// モデル読み込み
	modelManager_->LoadModel("Models/SampleBlock", "cube.obj");
	modelManager_->LoadModel("Models/DarumaBody", "DarumaBody.obj");
	collisionManager_ = std::make_unique<CollisionManager>();

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	cameraTargetPoint_.Initialize();
	followCamera_->SetParent(&cameraTargetPoint_);
	cameraTargetPoint_.translate = GlobalVariables::GetInstance()->GetVector3Value("FollowCamera", "OffsetPoint");

	GlobalVariables* global = GlobalVariables::GetInstance();
	global->CreateGroup("FollowCamera");
	global->AddItem("FollowCamera", "OffsetPoint", cameraTargetPoint_.translate);
	
	// プレイヤー
	player_ = std::make_unique<Player>();
	player_->SetCollisionManager(collisionManager_.get());
	player_->Initialize(followCamera_->GetCamera());
	player_->GenerateInitialize();
	// エネミー
	enemy_ = std::make_unique<Enemy>();

	terrain_ = std::make_unique<Terrain>();
	terrain_->Initialize();
	terrain_->SetCamera(followCamera_->GetCamera());
	terrain_->SetModel(modelManager_->FindModel("Models/SampleBlock", "cube.obj"));
	collisionManager_->SetColliderList(terrain_.get());
	terrain_->SetPosition(Vector3(0.0f, -20.0f, 0.0f));

	gameSystemManager_ = std::make_unique<GameSystemManager>();
	gameSystemManager_->Initialize(player_.get(),enemy_.get());

	// チュートリアル
	tutorial_ = std::make_unique<Tutorial>();
	tutorial_->Initialize();
	tutorial_->SetPlayer(player_.get());
	tutorial_->Start();
}

void GameScene::Update() {
#ifdef _DEBUG
	// ゲームのシステム
	ImGui::Begin("GameSystem");
	ImGui::DragFloat3("CameraPosition", &cameraTargetPoint_.translate.x, 0.01f);
	cameraTargetPoint_.translate = GlobalVariables::GetInstance()->GetVector3Value("FollowCamera", "OffsetPoint");
	gameSystemManager_->ImGuiDraw();
	ImGui::End();
	// プレイヤー
	player_->ImGuiDraw();
	// 地形
	terrain_->ImGuiDraw();
#endif // _DEBUG

	// シーンの切り替え処理

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		SceneTransition::GetInstance()->Start();
	}
#endif // _DEBUG
	if (gameSystemManager_->IsGameEnd()) {
		//SceneTransition::GetInstance()->Start();
	}
	if (SceneTransition::GetInstance()->GetSceneTransitionSignal()) {
		if (!tutorial_->GetIsStart() && !tutorial_->GetIsStart()) {
			sceneNum = TITLE_SCENE;
		}
	}

	// ゲームマネージャー側でゲームを止める処理
	if (gameSystemManager_->isGameStop_) {
		return;
	}

	gameSystemManager_->Update(tutorial_->GetIsStart());

	// チュートリアル
	tutorial_->Update();

	// 追従カメラ
	cameraTargetPoint_.UpdateMatrix();
	followCamera_->Update();
	// プレイヤー
	player_->Update(tutorial_->IsCountDown());

	// 地面
	terrain_->Update();

	// 当たり判定
	collisionManager_->CheckAllCollisions();

}

void GameScene::Draw() {
	player_->Draw();
	terrain_->Draw(textureHandle_);

	gameSystemManager_->Draw();
}

void GameScene::Finalize() {
	// ゲームパッドの振動を消す
	Input::GetInstance()->GamePadVibration(0, 0, 0);

	// 基底クラスの解放
	IScene::Finalize();
}