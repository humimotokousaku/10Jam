#pragma once
#include "Object3D.h"
#include "Collider.h"
#include "CollisionManager.h"

#include "Foot/FootCollider.h"

#include <memory>

struct PartParameter {
	float gravity;
};

/// <summary>
/// プレイヤーの
/// </summary>
class IPart : public Collider
{
public:
	// 衝突時に呼ばれる関数
	void OnCollision(Collider* collider) override{ collider; }
	// ワールド座標を取得
	Vector3 GetWorldPosition() override { return Vector3(object3D_->worldTransform.matWorld_.m[3][0], object3D_->worldTransform.matWorld_.m[3][1], object3D_->worldTransform.matWorld_.m[3][2]); };
	// 角度を取得
	Vector3 GetRotation() override{ return Vector3(); };
public:
	virtual void Initialize(CollisionManager* manager) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void ImGuiDraw() = 0;

	void SetModel(Model* model) { object3D_->SetModel(model); }
	void SetCamera(Camera* camera) { object3D_->SetCamera(camera); }
	void SetIsGround(bool isGround) { isGround_ = isGround; }
	WorldTransform* GetWorldTransform() { return &object3D_->worldTransform; }

protected:
	// 接地フラグ
	bool isGround_;
	// モデルと座標
	std::unique_ptr<Object3D> object3D_;
	// 足場
	std::unique_ptr<FootCollider> footCollider_;

};
