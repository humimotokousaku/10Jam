#include "FootCollider.h"
#include "GlobalVariables.h"
#include "../../../GameObjectLists.h"

void FootCollider::OnCollision(Collider* collider)
{
	if (GetTag() == collider->GetTag()) {
		return;
	}

	if (collider->GetCollisionAttribute() == kCollisionAttributeTerrain) {
		part_->SetIsGround(true);
		part_->SetIsTerrain(true);
	}
	else if(collider->GetCollisionAttribute() == kCollisionAttributeDarumaPart){
		part_->SetIsGround(true);
		part_->SetIsPart(true);
	}
}

void FootCollider::Initialize(IPart* part)
{
	part_ = part;
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize();
	object3D_->worldTransform.parent_ = part_->GetWorldTransform();
	object3D_->worldTransform.scale = Vector3(part_->GetWorldTransform()->scale.x, 0.25f, part_->GetWorldTransform()->scale.z);
	object3D_->worldTransform.translate.y -= 1.0f;

	SetCollisionPrimitive(kCollisionOBB);
	SetCollisionAttribute(kCollisionAttributeDarumaFoot);
	SetCollisionMask(~kCollisionAttributeDarumaFoot);
	SetTag(part_->partTag_);
	SetOBBLength(object3D_->worldTransform.scale);

	// 受け取り
#ifdef _DEBUG
	GlobalVariables* global = GlobalVariables::GetInstance();
	global->CreateGroup("FootCollider");
	global->AddItem("FootCollider", "ScaleY", object3D_->worldTransform.scale.y);
#endif // _DEBUG

	ApplyGlobalVariables();
}

void FootCollider::Update()
{
	ApplyGlobalVariables();
	part_->SetIsGround(false);
	object3D_->worldTransform.scale = Vector3(part_->GetWorldTransform()->scale.x, 0.25f, part_->GetWorldTransform()->scale.z);
	object3D_->worldTransform.UpdateMatrix();
}

void FootCollider::Draw()
{
	//object3D_->Draw();
}

void FootCollider::Object3DSetting(Camera* camera, Model* model)
{
	object3D_->SetCamera(camera);
	object3D_->SetModel(model);
}

void FootCollider::ApplyGlobalVariables()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	object3D_->worldTransform.scale.y = global->GetFloatValue("FootCollider", "ScaleY");
}
