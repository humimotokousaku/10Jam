#pragma once
#include "IScene.h"
#include "Audio.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "CollisionManager.h"
#include "Camera.h"

#include "../GameObjectLists.h"

#include <memory>

class GameManager;
class GameScene : public IScene
{
public:
	///
	/// Default Method
	/// 

	GameScene() = default;
	~GameScene()override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 解放処理
	/// </summary>
	void Finalize()override;

private:// プライベートなメンバ変数
	// 基本機能
	TextureManager* textureManager_;
	ModelManager* modelManager_;
	Audio* audio_;

private: // USER
	std::unique_ptr<CollisionManager> collisionManager_;
	// カメラ
	std::unique_ptr<Camera> camera_;

	std::unique_ptr<Player> player_;


};

