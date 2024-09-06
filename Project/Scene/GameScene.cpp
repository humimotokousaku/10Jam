#include "GameScene.h"
#include "GameManager.h"
#include "SceneTransition/SceneTransition.h"

void GameScene::Initialize() {
	sceneNum = GAME_SCENE;
	textureManager_ = TextureManager::GetInstance();
	modelManager_ = ModelManager::GetInstance();
	audio_ = Audio::GetInstance();

	gameTimer_.Initialize();
}

void GameScene::Update() {
	gameTimer_.Update();

	if (Input::GetInstance()->TriggerKey(DIK_SPACE) || Input::GetInstance()->GamePadTrigger(XINPUT_GAMEPAD_A)) {
		SceneTransition::GetInstance()->Start();
	}

	if (SceneTransition::GetInstance()->GetSceneTransitionSignal()) {
		sceneNum = GAMEOVER_SCENE;
	}
}

void GameScene::Draw() {
	
}

void GameScene::Finalize() {
	// ゲームパッドの振動を消す
	Input::GetInstance()->GamePadVibration(0, 0, 0);

	// 基底クラスの解放
	IScene::Finalize();
}