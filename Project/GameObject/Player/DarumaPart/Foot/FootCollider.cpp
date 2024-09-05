#include "FootCollider.h"
#include "../../../GameObjectLists.h"

void FootCollider::OnCollision(Collider* collider)
{
	if (part_->partTag_ == collider->GetTag()) {
		return;
	}

	if (collider->GetCollisionAttribute() == kCollisionAttributeTerrain) {
		part_->SetIsGround(true);
	}
	else if(collider->GetCollisionAttribute() == kCollisionAttributeDarumaPart){
		part_->SetIsGround(true);
	}
}

void FootCollider::Initialize(IPart* part)
{
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize();
	SetCollisionPrimitive(kCollisionOBB);
	SetCollisionAttribute(kCollisionAttributeDarumaFoot);
	SetCollisionMask(~kCollisionAttributeDarumaFoot);
	SetOBBLength(Vector3(1.0f, 0.2f, 1.0f));

	part_ = part;
	object3D_->worldTransform.parent_ = part_->GetWorldTransform();
	object3D_->worldTransform.translate.y -= 1.0f;
}

void FootCollider::Update()
{
	part_->SetIsGround(false);
	if (part_->GetWorldPosition().y <= -25.0f) {
		part_->SetIsGround(true);
	}

	object3D_->worldTransform.scale = GetOBB().m_fLength;
	object3D_->worldTransform.UpdateMatrix();
}

void FootCollider::Draw()
{
	object3D_->Draw();
}

void FootCollider::Object3DSetting(Camera* camera, Model* model)
{
	object3D_->SetCamera(camera);
	object3D_->SetModel(model);
}
