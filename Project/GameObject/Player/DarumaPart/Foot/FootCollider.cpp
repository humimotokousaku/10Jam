#include "FootCollider.h"
#include "../../../GameObjectLists.h"

void FootCollider::OnCollision(Collider* collider)
{
	if (GetTag() == collider->GetTag()) {
		return;
	}

	if (collider->GetCollisionAttribute() == kCollisionAttributeTerrain) {
		part_->SetIsGround(true);
		part_->isTerrain_ = true;
	}
	else if(collider->GetCollisionAttribute() == kCollisionAttributeDarumaPart){
		part_->SetIsGround(true);
	}
}

void FootCollider::Initialize(IPart* part)
{
	part_ = part;
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize();
	object3D_->worldTransform.parent_ = part_->GetWorldTransform();
	object3D_->worldTransform.scale = Vector3(part_->GetWorldTransform()->scale.x, 0.15f, part_->GetWorldTransform()->scale.z);
	object3D_->worldTransform.translate.y -= 1.0f;

	SetCollisionPrimitive(kCollisionOBB);
	SetCollisionAttribute(kCollisionAttributeDarumaFoot);
	SetCollisionMask(~kCollisionAttributeDarumaFoot);
	SetTag(part_->partTag_);
	SetOBBLength(object3D_->worldTransform.scale);
}

void FootCollider::Update()
{
	part_->SetIsGround(false);
	//if (part_->GetWorldPosition().y <= -25.0f) {
	//	part_->SetIsGround(true);
	//}

	//object3D_->worldTransform.scale = GetOBB().m_fLength;
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
