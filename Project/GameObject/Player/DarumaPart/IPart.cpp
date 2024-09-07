#include "IPart.h"
#include "Player/System/PhysicsSystem.h"

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

	//float mass = 20.0f;
	//float gravity = -9.8f;
	//float miu = 0.65f;
	//float magnitude = miu * (-mass * (gravity));
	//float frictForce = 0.0f;

	//if (std::fabsf(velocity_.x) > 0.01f) {
	//	Vector3 normalize = Normalize(velocity_);
	//	if (normalize.x > 0) {
	//		frictForce = magnitude * (-1.0f);
	//	}
	//	else if (normalize.x < 0) {
	//		frictForce = magnitude * (1.0f);
	//	}
	//}
	//else{
	//	velocity_.x = 0.0f;
	//}
	//acceleration_.x = frictForce / mass;
	//// 速度計算
	//velocity_.x += acceleration_.x * (1.0f / 60.0f);
	PlayerContext::PhysicsParam param;
	param.mass = 10.0f;
	param.frictionCoeff = 0.3f;
	param.gravity = 9.8f;
	param.normalForce = param.mass * param.gravity;

	// 速度を更新（加速度を考慮）
	velocity_ = PlayerContext::PhysicsSystem::ApplyX_ZFriction(velocity_, param);

	// 速度が小さい場合は停止とみなす
	if (std::fabsf(velocity_.x) < 0.01f) {
		velocity_.x = 0.0f;
	}
	if (std::fabsf(velocity_.z) < 0.01f) {
		velocity_.z = 0.0f;
	}

	object3D_->worldTransform.translate += velocity_;
	//velocity_ = Lerps::Lerp(velocity_, Vector3(0.0f, 0.0f, 0.0f), 0.01f);

	// 行列更新
	object3D_->worldTransform.UpdateMatrix();

	ColliderUpdate();
	footCollider_->Update();

	if (GetWorldPosition().y <= -40.0f) {
		isDead_ = true;
	}
}

void IPart::ImGuiDraw()
{
	ImGui::SeparatorText(partTag_.c_str());
	std::string name = "Position" + partTag_;
	if (ImGui::TreeNode("Transform")) {
		ImGui::DragFloat3(name.c_str(), &object3D_->worldTransform.translate.x, 0.01f);
		name = "Rotate" + partTag_;
		ImGui::DragFloat3(name.c_str(), &object3D_->worldTransform.rotate.x, 0.01f);
		name = "Scale" + partTag_;
		ImGui::DragFloat3(name.c_str(), &object3D_->worldTransform.scale.x, 0.01f);
		ImGui::TreePop();
	}
	name = "Velocity" + partTag_;
	ImGui::DragFloat3(name.c_str(), &velocity_.x);
	name = "IsGround : %d " + partTag_;
	ImGui::Text(name.c_str(), isGround_);
	name = "Index" + partTag_;
	int in = index_;
	ImGui::InputInt(name.c_str(), &in);
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
		object3D_->worldTransform.translate.y = correctPosition.y;
	}
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
