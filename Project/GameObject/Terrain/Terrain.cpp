#include "Terrain.h"
#include "GlobalVariables.h"

void Terrain::Initialize()
{
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize();
	object3D_->worldTransform.translate = Vector3(0.0f, 0.0f, 0.0f);
	object3D_->worldTransform.rotate = Vector3(0.0f, 0.0f, 0.0f);
	object3D_->worldTransform.scale = Vector3(50.0f, 3.0f, 50.0f);
	// コライダー登録
	SetCollisionPrimitive(kCollisionOBB);
	SetCollisionAttribute(kCollisionAttributeTerrain);
	SetCollisionMask(~kCollisionAttributeTerrain);
#ifdef _DEBUG
	GlobalVariables* global = GlobalVariables::GetInstance();
	global->CreateGroup("Terrain");
	global->AddItem("Terrain", "Scale", object3D_->worldTransform.scale);
#endif // _DEBUG
	ApplyGlobalVariables();
}

void Terrain::Update()
{
	ApplyGlobalVariables();
	SetOBBCenterPos(GetWorldPosition());
	SetOBBLength(object3D_->worldTransform.scale);

	object3D_->worldTransform.UpdateMatrix();
}

void Terrain::Draw()
{
	object3D_->Draw();
}

void Terrain::ImGuiDraw()
{

}

void Terrain::ApplyGlobalVariables()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	object3D_->worldTransform.scale = global->GetVector3Value("Terrain", "Scale");
}

void Terrain::OnCollision(Collider* collider)
{
	collider;
}
