#include "MediumPart.h"
#include "../Foot/FootCollider.h"
#include "ModelManager.h"
#include "ImGuiManager.h"

void MediumPart::Initialize(CollisionManager* manager)
{
	serialNumber_ = sSerialNumber;
	sSerialNumber++;
	partTag_ = "Medium" + std::to_string(serialNumber_);
	// 生成
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize();
	object3D_->worldTransform.translate = Vector3(0.0f, 0.0f, 0.0f);
	object3D_->worldTransform.scale = Vector3(2.0f, 2.0f, 2.0f);
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
	// 物理
	physics_.mass = 10.0f;
	physics_.frictionCoeff = 0.3f;
	physics_.gravity = 9.8f;
	physics_.normalForce = physics_.mass * physics_.gravity;
}

void MediumPart::Update()
{
	IPart::Update();
}

void MediumPart::Draw()
{
	// 描画
	object3D_->Draw();
	footCollider_->Draw();
}

void MediumPart::ImGuiDraw()
{
	IPart::ImGuiDraw();
}
