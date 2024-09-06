#include "IPart.h"

uint32_t IPart::sSerialNumber = 0;

void IPart::Update()
{
	if (!isGround_) {
		velocity_.y -= (4.5f * (1.0f / 60.0f));
		groundTimer_ = 0;
	}
	else {
		groundTimer_++;
		velocity_.y = 0.0f;
	}

	// 一番下の場合のみ
	if ((!isOtherFoot_ && isGround_) && index_ == 0 && groundTimer_ > 30) {
		//isDead_ = true;
		//return;
	}
	else if ((isTerrain_ && isGround_) && index_ != 0 && groundTimer_ > 30) {
		//isDead_ = true;
		//return;
	}
	isOtherFoot_ = false;
	isTerrain_ = false;

	float mass = 20.0f;
	float gravity = -9.8f;
	float miu = 0.65f;
	float magnitude = miu * (-mass * (gravity));
	float frictForce = 0.0f;

	if (std::fabsf(velocity_.x) > 0.01f) {
		Vector3 normalize = Normalize(velocity_);
		if (normalize.x > 0) {
			frictForce = magnitude * (-1.0f);
		}
		else if (normalize.x < 0) {
			frictForce = magnitude * (1.0f);
		}
	}
	else if(std::fabsf(velocity_.x) < 0.001f){
		velocity_.x = 0.0f;
	}
	acceleration_.x = frictForce / mass;

	if (std::fabsf(acceleration_.x / 60.0f) > std::fabsf(velocity_.x)) {
		acceleration_.x = velocity_.x * 60.0f;
	}

	// 速度計算
	velocity_.x += acceleration_.x * (1.0f / 60.0f);

	object3D_->worldTransform.translate += velocity_;
	//velocity_ = Lerps::Lerp(velocity_, Vector3(0.0f, 0.0f, 0.0f), 0.1f);

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
	// 最大最小
	Vector3 cMin = targetPosition - targetScale;
	Vector3 cMax = targetPosition + targetScale;

	bool isX = velocity_.x != 0.0f;
	bool isY = velocity_.y != 0.0f;
	bool isZ = velocity_.z != 0.0f;
	bool isMove = isX || isY || isZ;
	float correctValue = 0.00f;
	Vector3 correctPosition = GetWorldPosition();
	Vector3 pMin = correctPosition - object3D_->worldTransform.scale;
	Vector3 pMax = correctPosition + object3D_->worldTransform.scale;
	if (isY) {
		if (pMin.y >= cMin.y && pMin.y <= cMax.y) {
			correctPosition.y = targetPosition.y + object3D_->worldTransform.scale.y + targetScale.y + correctValue;
		}
		else if (pMax.y >= cMin.y && pMax.y <= cMax.y) {
			correctPosition.y = targetPosition.y - object3D_->worldTransform.scale.y - targetScale.y - correctValue;
		}
		object3D_->worldTransform.translate = correctPosition;
	}

	//if (collider->GetWorldPosition().y >= correctPosition.y + object3D_->worldTransform.scale.y && collider->GetWorldPosition().y - targetScale.y <= correctPosition.y + object3D_->worldTransform.scale.y) {
	//	correctPosition.y = targetPosition.y - object3D_->worldTransform.scale.y - targetScale.y - correctValue;
	//}
	//else if (collider->GetWorldPosition().y <= correctPosition.y - object3D_->worldTransform.scale.y && collider->GetWorldPosition().y + targetScale.y <= correctPosition.y - object3D_->worldTransform.scale.y) {
	//	correctPosition.y = targetPosition.y + object3D_->worldTransform.scale.y + targetScale.y + correctValue;
	//}
	//object3D_->worldTransform.translate = correctPosition;

	//if (isMove) {
	//	//if (isX) {
	//	//	// 右向き移動
	//	//	if (velocity_.x > 0.0f) {
	//	//		correctPosition.x = targetPosition.x - object3D_->worldTransform.scale.x - targetScale.x - correctValue;
	//	//	}
	//	//	// 左向き移動
	//	//	else {
	//	//		correctPosition.x = targetPosition.x + object3D_->worldTransform.scale.x + targetScale.x + correctValue;
	//	//	}
	//	//}
	//	if (isY) {
	//		// 上向き
	//		if (velocity_.y > 0.0f) {
	//			correctPosition.y = targetPosition.y - object3D_->worldTransform.scale.y - targetScale.y - correctValue;
	//		}
	//		// 下向き
	//		else if(velocity_.y > 0.0f){
	//			correctPosition.y = targetPosition.y + object3D_->worldTransform.scale.y + targetScale.y + correctValue;
	//		}
	//	}
	//	//if (isZ) {
	//	//	// 奥
	//	//	if (velocity_.z > 0.0f) {
	//	//		correctPosition.z = targetPosition.z - object3D_->worldTransform.scale.z - targetScale.z - correctValue;
	//	//	}
	//	//	// 手前
	//	//	else {
	//	//		correctPosition.z = targetPosition.z + object3D_->worldTransform.scale.z + targetScale.z + correctValue;
	//	//	}
	//	//}
	//	object3D_->worldTransform.translate = correctPosition;
	//}

}

void IPart::OnCollision(Collider* collider)
{
	bool isPart = collider->GetCollisionAttribute() == kCollisionAttributeDarumaPart;
	bool isTerrain = collider->GetCollisionAttribute() == kCollisionAttributeTerrain;
	bool isFoot = collider->GetCollisionAttribute() == kCollisionAttributeDarumaFoot;
	bool isCollision = (isPart || isTerrain);
	bool isTrue = collider->GetTag() == this->partTag_;
	if (isCollision && !isTrue) {
		CorrectPosition(collider);
	}
	else if(isFoot){
		if (!isTrue && index_ == 0) {
			isOtherFoot_ = true;
			return;
		}
	}
	if (isTerrain) {
		isTerrain_ = true;
	}
}
