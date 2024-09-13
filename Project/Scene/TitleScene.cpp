#include "TitleScene.h"
#include "SceneTransition/SceneTransition.h"
#include "GameSystemManager/GameSoundManager.h"

void TitleScene::Initialize() {
	sceneNum = TITLE_SCENE;

	// インスタンスを取得
	input_ = Input::GetInstance();
	modelManager_ = ModelManager::GetInstance();
	textureManager_ = TextureManager::GetInstance();

	GameSoundManager::GetInstance()->LoadAudio("Title", "Music/title.wav");
	GameSoundManager::GetInstance()->PlayBGMAudio("Title");
	// タイトルの演出
	titleEffect_.Initialize();
}

void TitleScene::Update() {
	// タイトル演出
	titleEffect_.Update();

	// シーンを切り替える合図があったら切り替える
	if (SceneTransition::GetInstance()->GetSceneTransitionSignal()) {
		sceneNum = GAME_SCENE;
		preSceneNum = TITLE_SCENE;
	}
}

void TitleScene::Draw() {
	// タイトル演出
	titleEffect_.Draw();
}

void TitleScene::Finalize() {
	// 基底クラスの解放
	IScene::Finalize();
}