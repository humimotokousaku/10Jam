#include "IPart.h"

uint32_t IPart::sSerialNumber = 0;

IPart::~IPart()
{
	collisionManager_->ClearColliderList(this);
	collisionManager_->ClearColliderList(footCollider_.get());
}

void IPart::Initialize(CollisionManager* manager)
{
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize();
	object3D_->worldTransform.translate = Vector3(0.0f, 0.0f, 0.0f);
	object3D_->worldTransform.rotate = Vector3(0.0f, 0.0f, 0.0f);

	collisionManager_ = manager;
	// コライダー登録
	SetCollisionPrimitive(kCollisionOBB);
	SetCollisionAttribute(kCollisionAttributeDarumaPart);
	SetCollisionMask(~kCollisionAttributePlayer);

	collisionManager_->SetColliderList(this);
}

void IPart::Update()
{
	// グローバルうんたら
	ApplyGlobalVariables();

	if (!isGround_) {
		velocity_.y -= (4.5f * GameSystemManager::sDeltaTime);
	}
	else {
		velocity_.y = 0.0f;
	}
	// 削除処理
	removeStatus_.Update(20);
	if (removeStatus_.isDelete) {
		isDead_ = true;
	}
	removeStatus_.FlagReset();

	// 速度を更新（加速度を考慮）
	velocity_ = PlayerContext::PhysicsSystem::ApplyX_ZFriction(velocity_, physics_);

	// 速度が小さい場合は停止とみなす
	if (std::fabsf(velocity_.x) < 0.01f) {
		velocity_.x = 0.0f;
	}
	if (std::fabsf(velocity_.z) < 0.01f) {
		velocity_.z = 0.0f;
	}
	// 座標計算
	object3D_->worldTransform.translate += velocity_;
	// 行列更新
	object3D_->worldTransform.UpdateMatrix();

	// コライダーの更新
	ColliderUpdate();
	footCollider_->Update();

	if (GetWorldPosition().y <= -40.0f) {
		isDead_ = true;
	}
}

void IPart::ImGuiDraw()
{
	ImGui::SeparatorText(partTag_.c_str());
	std::string name = "Transform" + partTag_;
	if (ImGui::TreeNode(name.c_str())) {
		name = "Position" + partTag_;
		ImGui::DragFloat3(name.c_str(), &object3D_->worldTransform.translate.x, 0.01f);
		name = "Rotate" + partTag_;
		ImGui::DragFloat3(name.c_str(), &object3D_->worldTransform.rotate.x, 0.01f);
		name = "Scale" + partTag_;
		ImGui::DragFloat3(name.c_str(), &object3D_->worldTransform.scale.x, 0.01f);
		ImGui::TreePop();
	}
	ImGui::Separator();
	name = "Velocity" + partTag_;
	ImGui::DragFloat3(name.c_str(), &velocity_.x);
	name = "IsGround : %d " + partTag_;
	ImGui::Text(name.c_str(), isGround_);
	name = "Index" + partTag_;
	int in = index_;
	ImGui::InputInt(name.c_str(), &in);
	name = "IsTerrain" + partTag_;
	ImGui::Checkbox(name.c_str(), &removeStatus_.isTerrain);
	name = "IsOverHead" + partTag_;
	ImGui::Checkbox(name.c_str(), &removeStatus_.isOverHead);
	name = "isPart" + partTag_;
	ImGui::Checkbox(name.c_str(), &removeStatus_.isPart);
}

void IPart::ApplyGlobalVariables()
{
	

}

void IPart::FootInitialize(CollisionManager* manager)
{
	footCollider_ = std::make_unique<FootCollider>();
	footCollider_->Initialize(this);
	manager->SetColliderList(footCollider_.get());
}

void IPart::ColliderUpdate()
{
	this->SetOBBCenterPos(GetWorldPosition());
	this->SetOBBLength(object3D_->worldTransform.scale);
	this->SetOBBDirect(0);
}

void IPart::CorrectPosition(Collider* collider)
{
	float minPenetrationDepth = (std::numeric_limits<float>::max)();
	Vector3 minPenetrationAxis;

	Vector3 axes[15];  // 衝突判定に使用する分離軸
	int axisCount = 0;

	// OBBの各軸を分離軸に追加
	for (int i = 0; i < 3; i++) {
		axes[axisCount++] = GetOBB().m_NormaDirect[i];
		axes[axisCount++] = collider->GetOBB().m_NormaDirect[i];
	}

	// OBBの各軸の外積を分離軸に追加
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			Vector3 crossAxis = Cross(GetOBB().m_NormaDirect[i], collider->GetOBB().m_NormaDirect[i]);
			if (Length(crossAxis) > 1e-6) {
				axes[axisCount++] = Normalize(crossAxis);
			}
		}
	}

	//Vector3 centerOffset = Subtract(obb2.center, obb1.center);
	Vector3 centerOffset = GetWorldPosition() - collider->GetWorldPosition();
	// 各軸に対して衝突判定
	for (int i = 0; i < axisCount; i++) {
		Vector3 axis = axes[i];

		// OBB1とOBB2の軸への投影半径を計算
		float r1 = GetOBB().m_fLength.x * std::fabsf(Dot(GetOBB().m_NormaDirect[0], axis)) +
			       GetOBB().m_fLength.y * std::fabsf(Dot(GetOBB().m_NormaDirect[1], axis)) +
			       GetOBB().m_fLength.z * std::fabsf(Dot(GetOBB().m_NormaDirect[2], axis));
		float r2 = collider->GetOBB().m_fLength.x * std::fabsf(Dot(collider->GetOBB().m_NormaDirect[0], axis)) +
			       collider->GetOBB().m_fLength.y * std::fabsf(Dot(collider->GetOBB().m_NormaDirect[1], axis)) +
			       collider->GetOBB().m_fLength.z * std::fabsf(Dot(collider->GetOBB().m_NormaDirect[2], axis));
		float distance = std::fabs(Dot(centerOffset, axis));

		// 貫通深度を計算
		float penetrationDepth = (r1 + r2) - distance;

		if (penetrationDepth < 0.0f) {
			continue;
		}

		// 最小の貫通深度を保存
		if (penetrationDepth < minPenetrationDepth) {
			minPenetrationDepth = penetrationDepth;
			minPenetrationAxis = Normalize(axis);
		}
	}

	// 中心オフセットとの内積を使用して、押し返し方向を決定
	if (Dot(centerOffset, minPenetrationAxis) < 0) {
		minPenetrationAxis = Multiply(-1, minPenetrationAxis);
	}

	// Y軸方向の押し返しを優先
	if (std::fabs(minPenetrationAxis.y) > std::fabs(minPenetrationAxis.x) &&
		std::fabs(minPenetrationAxis.y) > std::fabs(minPenetrationAxis.z)) {
		minPenetrationAxis = Vector3(0, minPenetrationAxis.y, 0);
	}



	object3D_->worldTransform.translate += Multiply(minPenetrationDepth * 0.5f, minPenetrationAxis);
}

void IPart::OnCollision(Collider* collider)
{
	bool isHead = collider->GetCollisionAttribute() == kCollisionAttributeDarumaHead;
	bool isPart = collider->GetCollisionAttribute() == kCollisionAttributeDarumaPart;
	bool isTerrain = collider->GetCollisionAttribute() == kCollisionAttributeTerrain;
	bool isFoot = collider->GetCollisionAttribute() == kCollisionAttributeDarumaFoot;
	bool isCollision = (isPart || isTerrain);
	bool isTrue = collider->GetTag() == this->partTag_;
	if (isCollision && !isTrue) {
		if (!isHead) {
			CorrectPosition(collider);
		}
	}
	if (isTerrain) {
		removeStatus_.isTerrain = isTerrain;
	}
	if ((isFoot && !isTrue)) {
		removeStatus_.isOverHead = true;
	}
	if (isPart && !isTrue) {
		removeStatus_.isPart = true;
	}
	object3D_->worldTransform.UpdateMatrix();
}

