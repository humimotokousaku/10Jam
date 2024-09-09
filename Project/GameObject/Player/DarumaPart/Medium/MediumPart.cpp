#include "MediumPart.h"
#include "../Foot/FootCollider.h"
#include "ModelManager.h"
#include "ImGuiManager.h"

void MediumPart::Initialize(CollisionManager* manager)
{
	// シリアル設定
	serialNumber_ = sSerialNumber;
	sSerialNumber++;
	partTag_ = "Medium" + std::to_string(serialNumber_);

	// 基底の初期化
	IPart::Initialize(manager);
	object3D_->worldTransform.scale = Vector3(2.0f, 2.0f, 2.0f);
	SetOBBLength(object3D_->worldTransform.scale);
	SetTag(partTag_);
	FootInitialize(manager);

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
