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
	textureManager_->LoadTexture("Models/DarumaHead", "DarumaHeadWeak.png");
	textureManager_->LoadTexture("Models/DarumaBody", "DarumaBodyBlue.png");
	textureManager_->LoadTexture("Models/DarumaBody", "DarumaBodyYellow.png");
	textureManager_->LoadTexture("Models/DarumaBody", "DarumaBodyGreen.png");
	textureManager_->LoadTexture("Models/DarumaBody", "DarumaBodyPurple.png");
	// テクスチャの番号を取得
	textureHandle_ = textureManager_->GetSrvIndex("DefaultTexture", "white.png");

	// モデル読み込み
	modelManager_->LoadModel("Models/SampleBlock", "cube.obj");
	modelManager_->LoadModel("Models/DarumaHead", "DarumaHead.obj");
	modelManager_->LoadModel("Models/DarumaBody", "DarumaBody.obj");
	collisionManager_ = std::make_unique<CollisionManager>();

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	cameraTargetPoint_.Initialize();

	followCamera_->SetParent(&cameraTargetPoint_);
	cameraTargetPoint_.translate = GlobalVariables::GetInstance()->GetVector3Value("FollowCamera", "OffsetPoint");
	cameraTargetPoint_.translate = { 0,0,0 };
	cameraTargetPoint_.UpdateMatrix();

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
	gameSystemManager_->Initialize(player_.get(), enemy_.get());

	// チュートリアル
	tutorial_ = std::make_unique<Tutorial>();
	tutorial_->Initialize();
	tutorial_->SetPlayer(player_.get());
	tutorial_->SetFollowCamera(followCamera_.get());
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
		sceneNum = TITLE_SCENE;
	}
#endif // _DEBUG
	if (gameSystemManager_->IsGameEnd()) {
		// 頭が地面につくとカメラ演出開始
		if (player_->GetPartManager()->GetDarumaHead()->IsGround() && player_->IsDead()) {
			followCamera_->GameOverAngle(&player_->GetPartManager()->GetDarumaHead()->object3D_->worldTransform, player_->GetPartManager()->GetDarumaHead()->object3D_->worldTransform.rotate);
		}
		// 落下で死んだ場合はすぐに遷移
		else {
			SceneTransition::GetInstance()->Start();
			preSceneNum = GAME_SCENE;
		}
		// 死亡時のカメラ演出が終了したら
		if (!followCamera_->CheckGameOverEffect()) {
			SceneTransition::GetInstance()->Start();
			preSceneNum = GAME_SCENE;
		}
	}
	if (SceneTransition::GetInstance()->GetSceneTransitionSignal()) {
		if (!tutorial_->GetIsStart()) {
			if (gameSystemManager_->IsGameClear()) {
				sceneNum = GAMECLEAR_SCENE;
				GameSystemManager::sClearPartCount = player_->GetPartManager()->parts_.size() - 1;
			}
			else if (gameSystemManager_->IsGameOver()) {
				sceneNum = GAMEOVER_SCENE;
			}
		}
	}

	// ゲームマネージャー側でゲームを止める処理
	if (gameSystemManager_->IsGameStop()) {
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

	// 押し返し
	player_->SolveConstraints();


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