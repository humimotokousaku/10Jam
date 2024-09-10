#include "MediumPart.h"
#include "../Foot/FootCollider.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "GlobalVariables.h"

void MediumPart::Initialize(CollisionManager* manager)
{
	// シリアル設定
	serialNumber_ = sSerialNumber;
	sSerialNumber++;
	partTag_ = "Medium" + std::to_string(serialNumber_);

	// 基底の初期化
	IPart::Initialize(manager);
	GlobalVariables* global = GlobalVariables::GetInstance();
	std::string groupName = "MediumPart";
	global->CreateGroup(groupName);
	Vector3 value = { 2.0f,1.5f,2.0f };
	global->AddItem(groupName, "Scale", value);
	object3D_->worldTransform.scale = global->GetVector3Value(groupName, "Scale");
	SetOBBLength(object3D_->worldTransform.scale);
	SetTag(partTag_);
	FootInitialize(manager);

	// USER
	isGround_ = false;
	// 物理
	global->AddItem(groupName, "Mass", 10.0f);
	global->AddItem(groupName, "FrictionCoeff", 0.3f);
	global->AddItem(groupName, "Gravity", 9.8f);
	physics_.mass = global->GetFloatValue(groupName, "Mass");
	physics_.frictionCoeff = global->GetFloatValue(groupName, "FrictionCoeff");
	physics_.gravity = global->GetFloatValue(groupName, "Gravity");
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

void MediumPart::ApplyGlobalVariables()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	std::string groupName = "MediumPart";
	object3D_->worldTransform.scale = global->GetVector3Value(groupName, "Scale");
	physics_.mass = global->GetFloatValue(groupName, "Mass");
	physics_.frictionCoeff = global->GetFloatValue(groupName, "FrictionCoeff");
	physics_.gravity = global->GetFloatValue(groupName, "Gravity");
}
