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
	// 物理
	physics_.mass = 20.0f;
	physics_.frictionCoeff = 0.4f;
	physics_.gravity = 9.8f;
	physics_.normalForce = physics_.mass * physics_.gravity;
}

void HeavyPart::Update()
{

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
	IPart::ImGuiDraw();
}
