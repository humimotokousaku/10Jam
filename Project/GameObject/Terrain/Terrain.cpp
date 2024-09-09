#include "Terrain.h"

void Terrain::Initialize()
{
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize();
	object3D_->worldTransform.translate = Vector3(0.0f, 0.0f, 0.0f);
	object3D_->worldTransform.scale = Vector3(200.0f, 3.0f, 50.0f);
	object3D_->worldTransform.rotate = Vector3(0.0f, 0.0f, 0.0f);
	// コライダー登録
	SetCollisionPrimitive(kCollisionOBB);
	SetCollisionAttribute(kCollisionAttributeTerrain);
	SetCollisionMask(~kCollisionAttributeTerrain);

}

void Terrain::Update()
{


	SetOBBCenterPos(GetWorldPosition());
	SetOBBLength(object3D_->worldTransform.scale);

	object3D_->worldTransform.UpdateMatrix();
}

void Terrain::Draw(uint32_t texture)
{
	object3D_->Draw(texture);
}

void Terrain::ImGuiDraw()
{

}

void Terrain::OnCollision(Collider* collider)
{
	collider;
}
