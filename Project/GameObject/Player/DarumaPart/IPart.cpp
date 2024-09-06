#include "IPart.h"

uint32_t IPart::sSerialNumber = 0;

void IPart::Update()
{
	//velocity_.y = 0.0f;
	object3D_->worldTransform.translate += velocity_;
	velocity_ = Lerps::Lerp(velocity_, Vector3(0.0f, 0.0f, 0.0f), 0.1f);

	// 行列更新
	object3D_->worldTransform.UpdateMatrix();

	ColliderUpdate();
	footCollider_->Update();

	if (GetWorldPosition().y <= -40.0f) {
		isDead_ = true;
	}
}

void IPart::ColliderUpdate()
{
	this->SetOBBCenterPos(GetWorldPosition());
	this->SetOBBDirect(0);
}

void IPart::CorrectPosition(Collider* collider)
{
	Vector3 targetPosition = collider->GetWorldPosition();
	Vector3 targetScale = collider->GetOBB().m_fLength;

	bool isX = velocity_.x != 0.0f;
	bool isY = velocity_.y != 0.0f;
	bool isZ = velocity_.z != 0.0f;
	bool isMove = isX || isY || isZ;
	float correctValue = 0.05f;
	Vector3 correctPosition = GetWorldPosition();
	if (isMove) {
		if (isX) {
			// 右向き移動
			if (velocity_.x > 0.0f) {
				correctPosition.x = targetPosition.x - object3D_->worldTransform.scale.x - targetScale.x - correctValue;
			}
			// 左向き移動
			else {
				correctPosition.x = targetPosition.x + object3D_->worldTransform.scale.x + targetScale.x + correctValue;
			}
		}
		if (isY) {
			// 上向き
			if (velocity_.y > 0.0f) {
				correctPosition.y = targetPosition.y - object3D_->worldTransform.scale.y - targetScale.y - correctValue;
			}
			// 下向き
			else {
				correctPosition.y = targetPosition.y + object3D_->worldTransform.scale.y + targetScale.y + correctValue;
			}
		}
		if (isZ) {
			// 奥
			if (velocity_.z > 0.0f) {
				correctPosition.z = targetPosition.z - object3D_->worldTransform.scale.z - targetScale.z - correctValue;
			}
			// 手前
			else {
				correctPosition.z = targetPosition.z + object3D_->worldTransform.scale.z + targetScale.z + correctValue;
			}
		}
		object3D_->worldTransform.translate = correctPosition;
	}

}

void IPart::OnCollision(Collider* collider)
{
	bool isPart = collider->GetCollisionAttribute() == kCollisionAttributeDarumaPart;
	bool isTerrain = collider->GetCollisionAttribute() == kCollisionAttributeTerrain;
	//bool is
	bool isCollision = (isPart || isTerrain);
	if (isCollision) {
		//CorrectPosition(collider);
	}
}
