#include "HeavyPart.h"
#include "Lerp.h"

void HeavyPart::Initialize(CollisionManager* manager)
{
	serialNumber_ = sSerialNumber;
	sSerialNumber++;
	partTag_ = "Heavy" + std::to_string(serialNumber_);

	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize();
	object3D_->worldTransform.translate = Vector3(0.0f, 0.0f, 0.0f);
	object3D_->worldTransform.scale = Vector3(3.0f, 3.0f, 3.0f);
	object3D_->worldTransform.rotate = Vector3(0.0f, 0.0f, 0.0f);
	// コライダー登録
	SetCollisionPrimitive(kCollisionOBB);
	SetCollisionAttribute(kCollisionAttributeDarumaPart);
	SetCollisionMask(~kCollisionAttributePlayer);
	SetOBBLength(object3D_->worldTransform.scale);

	SetTag(partTag_);

	manager->SetColliderList(this);
	footCollider_ = std::make_unique<FootCollider>();
	footCollider_->Initialize(this);
	manager->SetColliderList(footCollider_.get());


	// USER
	isGround_ = false;
}

void HeavyPart::Update()
{
	if (!isGround_) {
		object3D_->worldTransform.translate.y -= 9.8f * (1.0f / 60.0f);
	}

	IPart::Update();

}

void HeavyPart::Draw()
{
	// 描画
	object3D_->Draw();
	footCollider_->Draw();
}

void HeavyPart::ImGuiDraw()
{
	ImGui::Begin(partTag_.c_str());

	ImGui::DragFloat3("Position", &object3D_->worldTransform.translate.x, 0.01f);
	ImGui::DragFloat3("Rotate", &object3D_->worldTransform.rotate.x, 0.01f);
	ImGui::DragFloat3("Scale", &object3D_->worldTransform.scale.x, 0.01f);
	Vector3 obb = GetOBB().m_Pos;
	ImGui::DragFloat3("OBBPos", &obb.x);
	Vector3 obbLength = GetOBB().m_fLength;
	ImGui::DragFloat3("OBBLength", &obbLength.x);
	ImGui::End();
}

void HeavyPart::OnCollision(Collider* collider)
{
	if (collider->GetCollisionAttribute() == kCollisionAttributePlayer) {

	}
}
