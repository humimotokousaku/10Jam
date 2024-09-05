#pragma once
#include "Collider.h"
#include "Object3D.h"
#include <memory>

class Terrain : public Collider
{
public:
	void Initialize();
	void Update();
	void Draw(uint32_t texture);
	void ImGuiDraw();
public:
	void SetCamera(Camera* camera) { object3D_->SetCamera(camera); }
	void SetModel(Model* model) { object3D_->SetModel(model); }
	void SetPosition(const Vector3& position) { 
		object3D_->worldTransform.translate = position;
		object3D_->worldTransform.UpdateMatrix();
	}
public: // コライダー
	// 衝突時に呼ばれる関数
	// ワールド座標を取得
	Vector3 GetWorldPosition() override { return Vector3(object3D_->worldTransform.matWorld_.m[3][0], object3D_->worldTransform.matWorld_.m[3][1], object3D_->worldTransform.matWorld_.m[3][2]); };
	// 角度を取得
	Vector3 GetRotation() override { return Vector3(); };
	// 衝突時に呼ばれる関数
	void OnCollision(Collider* collider) override;
private:
	// モデルと座標
	std::unique_ptr<Object3D> object3D_;

};
