#include "LightPart.h"
#include "../Foot/FootCollider.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "GlobalVariables.h"

void LightPart::Initialize(CollisionManager* manager)
{
	// シリアル設定
	serialNumber_ = sSerialNumber;
	sSerialNumber++;
	partTag_ = "Light" + std::to_string(serialNumber_);

	// 基底の初期化
	IPart::Initialize(manager);
	GlobalVariables* global = GlobalVariables::GetInstance();
	std::string groupName = "LightPart";
	global->CreateGroup(groupName);
	Vector3 value = { 1.0f,1.5f,1.0f };
	global->AddItem(groupName, "Scale", value);
	object3D_->worldTransform.scale = global->GetVector3Value(groupName, "Scale");
	SetOBBLength(object3D_->worldTransform.scale);
	SetTag(partTag_);
	FootInitialize(manager);

	// USER
	isGround_ = false;
	// 物理
	global->AddItem(groupName, "Mass", 7.5f);
	global->AddItem(groupName, "FrictionCoeff", 0.3f);
	global->AddItem(groupName, "Gravity", 9.8f);
	physics_.mass = global->GetFloatValue(groupName, "Mass");
	physics_.frictionCoeff = global->GetFloatValue(groupName, "FrictionCoeff");
	physics_.gravity = global->GetFloatValue(groupName, "Gravity");
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

void LightPart::ApplyGlobalVariables()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	std::string groupName = "LightPart";
	object3D_->worldTransform.scale = global->GetVector3Value(groupName, "Scale");
	physics_.mass = global->GetFloatValue(groupName, "Mass");
	physics_.frictionCoeff = global->GetFloatValue(groupName, "FrictionCoeff");
	physics_.gravity = global->GetFloatValue(groupName, "Gravity");
	physics_.normalForce = physics_.mass * physics_.gravity;
}
