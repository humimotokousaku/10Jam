#pragma once
#include "Object3D.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "Player/System/PhysicsSystem.h"

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
	static uint32_t sSerialNumber;
public:
	uint32_t serialNumber_;
	std::string partTag_;
public: // 仮想関数
	// デストラクタ
	virtual ~IPart() = default;
	virtual void Initialize(CollisionManager* manager);
	virtual void Update();
	virtual void Draw() = 0;
	virtual void ImGuiDraw();
	virtual void ApplyGlobalVariables();
protected: // 継承先で使用する関数
	void FootInitialize(CollisionManager* manager);
	void ColliderUpdate();
	void CorrectPosition(Collider* collider);

public: // アクセッサ
	// モデル
	void SetModel(Model* model) { object3D_->SetModel(model); }
	// カメラ
	void SetCamera(Camera* camera) { object3D_->SetCamera(camera); }
	// 接地
	void SetIsGround(bool isGround) { isGround_ = isGround; }
	// 死
	void SetIsDead(bool isDead) { isDead_ = isDead; }
	// 座標
	void SetPosition(const Vector3& position) {
		object3D_->worldTransform.translate = position;
		object3D_->worldTransform.UpdateMatrix();
	}

	WorldTransform* GetWorldTransform() { return &object3D_->worldTransform; }
	FootCollider* GetFootCollider() { return footCollider_.get(); }
	bool IsGround() { return isGround_; }
	bool IsDead() { return isDead_; }
public: // コライダー
	// 衝突時に呼ばれる関数
	// ワールド座標を取得
	Vector3 GetWorldPosition() override { return Vector3(object3D_->worldTransform.matWorld_.m[3][0], object3D_->worldTransform.matWorld_.m[3][1], object3D_->worldTransform.matWorld_.m[3][2]); };
	// 角度を取得
	Vector3 GetRotation() override { return Vector3(object3D_->worldTransform.rotate); };
	void OnCollision(Collider* collider) override;

protected:
	// 接地フラグ
	bool isGround_;
	// 消すフラグ
	bool isDead_;
	// 足場
	std::unique_ptr<FootCollider> footCollider_;
	// 物理のパラメータ
	PlayerContext::PhysicsParam physics_;
public:
	// モデルと座標
	std::unique_ptr<Object3D> object3D_;
	// 速さ
	Vector3 velocity_;
	Vector3 acceleration_{};
	uint32_t index_ = 0;

	bool isOtherFoot_ = true;
	bool isTerrain_ = true;

	uint32_t groundTimer_ = 0;
};
