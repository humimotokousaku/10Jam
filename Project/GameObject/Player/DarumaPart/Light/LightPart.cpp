#include "LightPart.h"
#include "../Foot/FootCollider.h"
#include "ModelManager.h"
#include "ImGuiManager.h"

void LightPart::Initialize(CollisionManager* manager)
{
	// 生成
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize();
	object3D_->worldTransform.translate = Vector3(0.0f, 0.0f, 0.0f);
	object3D_->worldTransform.scale = Vector3(1.0f, 1.0f, 1.0f);
	object3D_->worldTransform.rotate = Vector3(0.0f, 0.0f, 0.0f);
	// コライダー登録
	SetCollisionPrimitive(kCollisionOBB);
	SetCollisionAttribute(kCollisionAttributeTerrain);
	SetCollisionMask(~kCollisionAttributeTerrain);
	manager->SetColliderList(this);
	footCollider_ = std::make_unique<FootCollider>();
	footCollider_->Initialize(this);
	manager->SetColliderList(footCollider_.get());

	// USER
	isGround_ = false;

}

void LightPart::Update()
{
	if (!isGround_) {
		object3D_->worldTransform.translate.y -= 9.8f * (1.0f / 60.0f);
	}
	// 行列更新
	object3D_->worldTransform.UpdateMatrix();

	ColliderUpdate();
	footCollider_->Update();
}

void LightPart::Draw()
{
	// 描画
	object3D_->Draw();
}

void LightPart::ImGuiDraw()
{
	ImGui::Begin("LightPart");

	ImGui::DragFloat3("Position", &object3D_->worldTransform.translate.x, 0.01f);
	ImGui::DragFloat3("Rotate", &object3D_->worldTransform.rotate.x, 0.01f);
	ImGui::DragFloat3("Scale", &object3D_->worldTransform.scale.x, 0.01f);
	Vector3 obb = GetOBB().m_Pos;
	ImGui::DragFloat3("OBBPos", &obb.x);
	ImGui::End();
}

void LightPart::OnCollision(Collider* collider)
{
	if (collider->GetCollisionAttribute() == kCollisionAttributePlayer) {

	}


}
