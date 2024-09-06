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
	static uint32_t sSerialNumber;
public:
	uint32_t serialNumber_;
	std::string partTag_;
public: // 仮想関数
	// デストラクタ
	virtual ~IPart() = default;
	virtual void Initialize(CollisionManager* manager) = 0;
	virtual void Update();
	virtual void Draw() = 0;
	virtual void ImGuiDraw() = 0;
protected: // 継承先で使用する関数
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

protected:
	// 接地フラグ
	bool isGround_;
	// 消すフラグ
	bool isDead_;
	// 足場
	std::unique_ptr<FootCollider> footCollider_;
public:
	// モデルと座標
	std::unique_ptr<Object3D> object3D_;
	// 速さ
	Vector3 velocity_;
};
