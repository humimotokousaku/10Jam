#include "LightPart.h"
#include "ModelManager.h"

void LightPart::Initialize(CollisionManager* manager)
{
	// 生成
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize();
	object3D_->worldTransform.translate = Vector3(0.0f, 0.0f, 0.0f);
	object3D_->worldTransform.scale = Vector3(1.0f, 1.0f, 1.0f);
	object3D_->worldTransform.rotate = Vector3(0.0f, 0.0f, 0.0f);
	// コライダー登録
	SetCollisionPrimitive(kCollisionOBB);
	SetCollisionAttribute(kCollisionAttributeTerrain);
	SetCollisionMask(~kCollisionAttributeTerrain);
	manager->SetColliderList(this);
}

void LightPart::Update()
{
	// 行列更新
	object3D_->worldTransform.UpdateMatrix();
}

void LightPart::Draw()
{
	// 描画
	object3D_->Draw();
}
