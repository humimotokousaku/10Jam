#pragma once
#include <memory>
#include <vector>
#include "ModelManager.h"

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

	template<typename T>
	void AddParts();

public: // アクセッサ
	void SetCollisionManager(CollisionManager* collisionManager) { collisionManager_ = collisionManager; }

private:
	// 土台のリスト
	std::vector<std::unique_ptr<IPart>> parts_;
	std::vector<std::unique_ptr<IPart>>::iterator partsIt_;
	// ポインタ
	Camera* camera_ = nullptr;
	// コライダーのマネージャ
	CollisionManager* collisionManager_ = nullptr;
	// 押す力ベクトル
	Vector3 pushPower_;

};

template<typename T>
inline void Player::AddParts()
{
	std::unique_ptr<IPart> instance = std::make_unique<T>();
	instance->Initialize(collisionManager_);
	instance->SetCamera(camera_);
	instance->SetModel(ModelManager::GetInstance()->FindModel("Models/SampleBlock", "cube.obj"));
	instance->GetFootCollider()->Object3DSetting(camera_, ModelManager::GetInstance()->FindModel("Models/SampleBlock", "cube.obj"));
	parts_.push_back(std::move(instance));
}
