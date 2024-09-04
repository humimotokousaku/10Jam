#pragma once
#include "Collider.h"
#include "Object3D.h"

#include <memory>

class IPart;

/// <summary>
/// 足元コライダー
/// </summary>
class FootCollider : public Collider
{
public:
	// 衝突時に呼ばれる関数
	void OnCollision(Collider* collider) override;
	// ワールド座標を取得
	Vector3 GetWorldPosition() override {
		return Vector3(object3D_->worldTransform.matWorld_.m[3][0], object3D_->worldTransform.matWorld_.m[3][1], object3D_->worldTransform.matWorld_.m[3][2]);
	}
	// 角度を取得
	Vector3 GetRotation() override { return Vector3(); }

public:
	void Initialize(IPart* part);
	void Update();
private:
	//
	std::unique_ptr<Object3D> object3D_;
	// パーツの
	IPart* part_;

};