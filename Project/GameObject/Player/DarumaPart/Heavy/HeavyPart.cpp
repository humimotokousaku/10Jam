#include "HeavyPart.h"
#include "Lerp.h"
#include "GlobalVariables.h"

HeavyPart::~HeavyPart() {
	collisionManager_->ClearColliderList(this);
}

void HeavyPart::Initialize(CollisionManager* manager)
{
	collisionManager_ = manager;
	// シリアル設定
	serialNumber_ = sSerialNumber;
	sSerialNumber++;
	partTag_ = "Heavy" + std::to_string(serialNumber_);

	// 基底の初期化
	IPart::Initialize(manager);
	GlobalVariables* global = GlobalVariables::GetInstance();
	global->CreateGroup("HeavyPart");
	Vector3 value = { 3.0f,1.5f,3.0f };
	global->AddItem("HeavyPart", "Scale", value);
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
	GlobalVariables* global = GlobalVariables::GetInstance();
	object3D_->worldTransform.scale = global->GetVector3Value("HeavyPart", "Scale");
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
	object3D_->worldTransform.scale = global->GetVector3Value("HeavyPart", "Scale");
}
