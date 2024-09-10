#include "HeadPart.h"
#include "TextureManager.h"
#include "GlobalVariables.h"

void HeadPart::Initialize(CollisionManager* manager)
{
	// シリアル設定
	serialNumber_ = sSerialNumber;
	sSerialNumber++;
	partTag_ = "Head";

	// 基底の初期化
	IPart::Initialize(manager);
	GlobalVariables* global = GlobalVariables::GetInstance();
	global->CreateGroup("HeadPart");
	Vector3 value = { 2.5f,2.5f,2.5f };
	global->AddItem("HeadPart", "Scale", value);
	object3D_->worldTransform.scale = global->GetVector3Value("HeadPart", "Scale");
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

	texture_ = TextureManager::GetInstance()->GetSrvIndex("", "uvChecker.png");
}

void HeadPart::Update()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	object3D_->worldTransform.scale = global->GetVector3Value("HeadPart", "Scale");
	IPart::Update();
}

void HeadPart::Draw()
{
	// 描画
	object3D_->Draw(texture_);
	footCollider_->Draw();
}

void HeadPart::ImGuiDraw()
{
	IPart::ImGuiDraw();
}

void HeadPart::ApplyGlobalVariables()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	object3D_->worldTransform.scale = global->GetVector3Value("HeadPart", "Scale");
}
