#include "LightPart.h"
#include "../Foot/FootCollider.h"
#include "ModelManager.h"
#include "ImGuiManager.h"

void LightPart::Initialize(CollisionManager* manager)
{
	// シリアル設定
	serialNumber_ = sSerialNumber;
	sSerialNumber++;
	partTag_ = "Light" + std::to_string(serialNumber_);

	// 基底の初期化
	IPart::Initialize(manager);
	object3D_->worldTransform.scale = Vector3(1.0f, 1.0f, 1.0f);
	SetOBBLength(object3D_->worldTransform.scale);
	SetTag(partTag_);
	FootInitialize(manager);

	// USER
	isGround_ = false;
	// 物理
	physics_.mass = 7.5f;
	physics_.frictionCoeff = 0.3f;
	physics_.gravity = 9.8f;
	physics_.normalForce = physics_.mass * physics_.gravity;
}

void LightPart::Update()
{
	IPart::Update();
}

void LightPart::Draw()
{
	// 描画
	object3D_->Draw();
	footCollider_->Draw();
}

void LightPart::ImGuiDraw()
{
	IPart::ImGuiDraw();
}
