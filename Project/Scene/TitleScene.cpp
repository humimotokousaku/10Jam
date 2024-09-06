#include "TitleScene.h"
#include "SceneTransition/SceneTransition.h"

void TitleScene::Initialize() {
	sceneNum = TITLE_SCENE;
	// インスタンスを取得
	input_ = Input::GetInstance();
	modelManager_ = ModelManager::GetInstance();
	textureManager_ = TextureManager::GetInstance();


	/// カメラの作成
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();


	// 衝突マネージャーを生成
	collisionManager_ = std::make_unique<CollisionManager>();


	/// テクスチャの読み込み
	textureManager_->LoadTexture("", "uvChecker.png");
	// テクスチャの番号を取得
	textureHandle_ = textureManager_->GetSrvIndex("", "uvChecker.png");

	/// 使用するモデルの読み込み
	modelManager_->LoadModel("Models/SampleBlock", "cube.obj");
	// ISceneクラスにあるmodels_変数にモデル情報を格納する(使用しなくても問題なし)
	// models_[0]に登録
	// 次に登録したらmodels_[1]に登録される
	AddModel(modelManager_->SetModel("Models/SampleBlock", "cube.obj"));


	// テスト用のブロック
	for (int i = 0; i < sampleObject_.size(); i++) {
		sampleObject_[i] = std::make_unique<SampleObject>();
		sampleObject_[i]->AddModel(models_[0]);
		sampleObject_[i]->SetCamera(camera_.get());
		sampleObject_[i]->SetCollisionManager(collisionManager_.get());
		sampleObject_[i]->Init();
	}

	sampleObject_[0]->SetPosiotion(Vector3{ -3.0f,-3.0f,10.0f });
	sampleObject_[1]->SetPosiotion(Vector3{ 3.0f,-3.0f,10.0f });

	// UIのスプライトを作成
	guideUI_[0] = std::make_unique<Sprite>();
	guideUI_[0]->Initialize("Textures/BackGround", "gray.png");
	guideUI_[0]->SetSize(Vector2{ 1280,720 });
	guideUI_[0]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 2 });
	guideUI_[1] = std::make_unique<Sprite>();
	guideUI_[1]->Initialize("Textures/UI", "titleName.png");
	guideUI_[1]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 4 });
	guideUI_[2] = std::make_unique<Sprite>();
	guideUI_[2]->Initialize("Textures/UI", "guide_pad_A.png");
	guideUI_[2]->SetSize(Vector2{ 64,64 });
	guideUI_[2]->SetPos(Vector2{ (float)WinApp::kClientWidth_ / 2,(float)WinApp::kClientHeight_ / 4 * 3 });
	for (int i = 0; i < 3; i++) {
		PostEffectManager::GetInstance()->AddSpriteList(guideUI_[i].get());
	}
}

void TitleScene::Update() {
	// テスト用ブロック
	for (int i = 0; i < sampleObject_.size(); i++) {
		sampleObject_[i]->Update();
		sampleObject_[i]->ImGuiParam(i);
	}


	// カメラ
	camera_->Update();


	// 衝突マネージャー
	collisionManager_->CheckAllCollisions();


	// シーン遷移演出開始する
	if (input_->TriggerKey(DIK_SPACE) || input_->GamePadTrigger(XINPUT_GAMEPAD_A)) {
		SceneTransition::GetInstance()->Start();
	}
	// シーンを切り替える合図があったら切り替える
	if (SceneTransition::GetInstance()->GetSceneTransitionSignal()) {
		sceneNum = GAME_SCENE;
	}
}

void TitleScene::Draw() {
	// テスト用ブロック
	for (int i = 0; i < sampleObject_.size(); i++) {
		sampleObject_[i]->Draw();
	}
}

void TitleScene::Finalize() {
	// 基底クラスの解放
	IScene::Finalize();
}