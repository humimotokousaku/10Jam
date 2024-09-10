#include "HeavyPart.h"
#include "Lerp.h"
#include "GlobalVariables.h"

void HeavyPart::Initialize(CollisionManager* manager)
{
	// シリアル設定
	serialNumber_ = sSerialNumber;
	sSerialNumber++;
	partTag_ = "Heavy" + std::to_string(serialNumber_);

	// 基底の初期化
	IPart::Initialize(manager);
	GlobalVariables* global = GlobalVariables::GetInstance();
	std::string groupName = "HeavyPart";
	global->CreateGroup(groupName);
	Vector3 value = { 3.0f,1.5f,3.0f };
	global->AddItem(groupName, "Scale", value);
	object3D_->worldTransform.scale = global->GetVector3Value(groupName, "Scale");
	SetOBBLength(object3D_->worldTransform.scale);
	SetTag(partTag_);
	FootInitialize(manager);

	// USER
	isGround_ = false;
	// 物理
	global->AddItem(groupName, "Mass", 20.0f);
	global->AddItem(groupName, "FrictionCoeff", 0.4f);
	global->AddItem(groupName, "Gravity", 9.8f);
	physics_.mass = global->GetFloatValue(groupName, "Mass");
	physics_.frictionCoeff = global->GetFloatValue(groupName, "FrictionCoeff");
	physics_.gravity = global->GetFloatValue(groupName, "Gravity");
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

void HeavyPart::ApplyGlobalVariables()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	std::string groupName = "HeavyPart";
	object3D_->worldTransform.scale = global->GetVector3Value(groupName, "Scale");
	physics_.mass = global->GetFloatValue(groupName, "Mass");
	physics_.frictionCoeff = global->GetFloatValue(groupName, "FrictionCoeff");
	physics_.gravity = global->GetFloatValue(groupName, "Gravity");
}
