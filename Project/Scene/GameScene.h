#pragma once
#include "IScene.h"
#include "Audio.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "FollowCamera/FollowCamera.h"

#include "../GameObjectLists.h"
#include "../GameSystemManager/GameSystemManager.h"
#include "../AttackDirection/AttackDirection.h"

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
	// テクスチャの番号
	uint32_t textureHandle_;
private: // USER
	std::unique_ptr<CollisionManager> collisionManager_;
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	// 自機
	std::unique_ptr<Player> player_;
	// 敵
	std::unique_ptr<Enemy> enemy_;
	// 地面
	std::unique_ptr<Terrain> terrain_;
	// ゲームのシステム
	std::unique_ptr<GameSystemManager> gameSystemManager_;

	// 追従カメラの見ている座標
	WorldTransform cameraTargetPoint_;
};

