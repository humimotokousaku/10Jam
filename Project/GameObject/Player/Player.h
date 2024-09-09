#pragma once
#include <memory>
#include <vector>
#include "ModelManager.h"
#include "System/PartManager.h"

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
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// ImGuiの処理
	/// </summary>
	void ImGuiDraw();

public: // アクセッサ
	void SetCollisionManager(CollisionManager* collisionManager) { collisionManager_ = collisionManager; }
	void SetPushPower(Vector3 pushPower) { pushPower_ = pushPower; }
	PlayerContext::PartManager GetPartManager() { return partManager_; }
	std::vector<std::unique_ptr<IPart>>* GetParts() { return &parts_; }
	std::vector<std::unique_ptr<IPart>>::iterator* GetPartsIterator() { return &partsIt_; }
	Camera* GetCamera() { return camera_; }
	// コライダーのマネージャ
	CollisionManager* GetCollision() { return collisionManager_; }
	std::vector<std::unique_ptr<IPart>> parts_;

private:
	// 土台のリスト
	std::vector<std::unique_ptr<IPart>>::iterator partsIt_;
	// ポインタ
	Camera* camera_ = nullptr;
	// コライダーのマネージャ
	CollisionManager* collisionManager_ = nullptr;
	// 押す力ベクトル
	Vector3 pushPower_;
	Vector3 generatePosition_;

	PlayerContext::PartManager partManager_;

};
