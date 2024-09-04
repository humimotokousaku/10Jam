#include "FootCollider.h"
#include "../../../GameObjectLists.h"

void FootCollider::OnCollision(Collider* collider)
{
	collider;
}

void FootCollider::Initialize(IPart* part)
{
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize();

	SetCollisionPrimitive(kCollisionOBB);
	SetCollisionAttribute(kCollisionAttributeTerrain);
	SetCollisionMask(~kCollisionAttributeTerrain);
	//object3D_->SetModel();

	part_ = part;
	object3D_->worldTransform.parent_ = part_->GetWorldTransform();
}

void FootCollider::Update()
{
	part_->SetIsGround(false);
	if (part_->GetWorldPosition().y <= -5.0f) {
		part_->SetIsGround(true);
	}

}
