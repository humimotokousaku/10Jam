#pragma once
#include <memory>
#include <vector>
#include "ModelManager.h"
#include "System/PartManager.h"
#include "System/ReactionManager.h"

class IPart;
class Camera;
class CollisionManager;

class Player
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera);
	/// <summary>
	/// 更新
	/// </summary>
	void Update(bool isCountDown);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// ImGuiの処理
	/// </summary>
	void ImGuiDraw();
	/// <summary>
	/// 生成初期化
	/// </summary>
	void GenerateInitialize();
	/// <summary>
	/// 衝突解決処理
	/// </summary>
	void SolveConstraints();

public: // アクセッサ
	void SetCollisionManager(CollisionManager* collisionManager) { collisionManager_ = collisionManager; }
	void SetIsDead(bool isDead) { isDead_ = isDead; }	
	bool IsDead() { return isDead_; }

	Camera* GetCamera() { return camera_; }
	// コライダーのマネージャ
	CollisionManager* GetCollision() { return collisionManager_; }

	PlayerContext::ReactionManager* GetReactionManager() { return &reactionManager_; }
	PlayerContext::PartManager* GetPartManager() { return &partManager_; }

private:
	// ポインタ
	Camera* camera_ = nullptr;
	// コライダーのマネージャ
	CollisionManager* collisionManager_ = nullptr;
	Vector3 generatePosition_;

	// パーツ
	PlayerContext::PartManager partManager_;
	// リアクション
	PlayerContext::ReactionManager reactionManager_;

	bool isDead_ = false;

};
