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
		velocity_.y = -(9.8f * (1.0f / 60.0f));
	}
	else {
		velocity_.y = 0.0f;
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
	ImGui::SeparatorText(partTag_.c_str());
	std::string name = "Position" + partTag_;
	ImGui::DragFloat3(name.c_str(), &object3D_->worldTransform.translate.x, 0.01f);
	name = "Rotate" + partTag_;
	ImGui::DragFloat3(name.c_str(), &object3D_->worldTransform.rotate.x, 0.01f);
	name = "Scale" + partTag_;
	ImGui::DragFloat3(name.c_str(), &object3D_->worldTransform.scale.x, 0.01f);
	Vector3 obb = GetOBB().m_Pos;
	name = "OBBPosition" + partTag_;
	ImGui::DragFloat3(name.c_str(), &obb.x);
	Vector3 obbLength = GetOBB().m_fLength;
	name = "OBBLength" + partTag_;
	ImGui::DragFloat3(name.c_str(), &obbLength.x);
	name = "Index" + partTag_;
	int in = index_;
	ImGui::InputInt(name.c_str(), &in);
}
