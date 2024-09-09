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
	global->CreateGroup("HeavyPart");
	global->AddItem("HeavyPart", "Scale", object3D_->worldTransform.scale);
	object3D_->worldTransform.scale = global->GetVector3Value("HeavyPart", "Scale");
	SetOBBLength(object3D_->worldTransform.scale);
	SetTag(partTag_);
	FootInitialize(manager);

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
