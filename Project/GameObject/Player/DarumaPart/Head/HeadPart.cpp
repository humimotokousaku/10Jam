#include "HeadPart.h"
#include "TextureManager.h"
#include "GlobalVariables.h"
#include "GameObjectLists.h"

void HeadPart::Initialize(CollisionManager* manager)
{
	// シリアル設定
	serialNumber_ = sSerialNumber;
	sSerialNumber++;
	partTag_ = "Head";

	// 基底の初期化
	IPart::Initialize(manager);
	SetCollisionAttribute(kCollisionAttributeDarumaHead);

	GlobalVariables* global = GlobalVariables::GetInstance();
	std::string groupName = "HeadPart";
	global->CreateGroup(groupName);
	Vector3 value = { 2.5f,2.5f,2.5f };
	global->AddItem(groupName, "Scale", value);
	object3D_->worldTransform.scale = global->GetVector3Value(groupName, "Scale");
	SetOBBLength(object3D_->worldTransform.scale);
	SetTag(partTag_);
	FootInitialize(manager);

	// USER
	isGround_ = false;
	// 物理
	global->AddItem(groupName, "Mass", 10.0f);
	global->AddItem(groupName, "FrictionCoeff", 0.3f);
	global->AddItem(groupName, "Gravity", 9.8f);
	physics_.mass = global->GetFloatValue(groupName, "Mass");
	physics_.frictionCoeff = global->GetFloatValue(groupName, "FrictionCoeff");
	physics_.gravity = global->GetFloatValue(groupName, "Gravity");
	physics_.normalForce = physics_.mass * physics_.gravity;

	texture_ = TextureManager::GetInstance()->GetSrvIndex("", "uvChecker.png");
}

void HeadPart::Update()
{
	// グローバルうんたら
	ApplyGlobalVariables();

	//if (!isGround_) {
	//	velocity_.y -= (4.5f * (1.0f / 60.0f));
	//	// 上の段
	//	if (index_ != 0) {
	//		groundTimer_ = 0;
	//	}
	//	// 下の段
	//	else if (index_ == 0 && isOtherFoot_) {
	//		groundTimer_++;
	//	}
	//}
	//else {
	//	velocity_.y = 0.0f;
	//	if (velocity_.x == 0.0f || velocity_.z == 0.0f) {
	//		if (index_ != 0) {
	//			groundTimer_++;
	//		}
	//		else if (index_ == 0 && !isOtherFoot_) {
	//			groundTimer_++;
	//		}
	//	}
	//}

	// 速度を更新（加速度を考慮）
	velocity_ = PlayerContext::PhysicsSystem::ApplyX_ZFriction(velocity_, physics_);

	// 速度が小さい場合は停止とみなす
	if (std::fabsf(velocity_.x) < 0.01f) {
		velocity_.x = 0.0f;
	}
	if (std::fabsf(velocity_.z) < 0.01f) {
		velocity_.z = 0.0f;
	}

	object3D_->worldTransform.translate += velocity_;

	// 行列更新
	object3D_->worldTransform.UpdateMatrix();

	ColliderUpdate();
	footCollider_->Update();

	if (GetWorldPosition().y <= -40.0f) {
		isDead_ = true;
	}
	IPart::Update();
}

void HeadPart::Draw()
{
	// 描画
	object3D_->Draw(texture_);
	footCollider_->Draw();
}

void HeadPart::ImGuiDraw()
{
	IPart::ImGuiDraw();
}

void HeadPart::AddTorque(Collider* collider)
{
	/*Vector3 min = collider->GetWorldPosition() - collider->GetOBB().m_fLength;
	Vector3 max = collider->GetWorldPosition() + collider->GetOBB().m_fLength;*/

	Vector3 diff = Normalize(GetWorldPosition() - collider->GetWorldPosition());
	float dot = Dot({0.0f, 1.0f, 0.0f}, diff);
	float lenght = Length({ 0.0f, 1.0f, 0.0f }) * Length(diff);

	if (lenght != 0) {
		float cosTheta = dot / lenght;
		cosTheta = std::fmax(-1.0f, std::fmin(1.0f, cosTheta));

		float angleRadians = std::acos(cosTheta);
		float angle = angleRadians * float(180.0f / std::numbers::pi);
		if (angleRadians * float(180.0f / std::numbers::pi) > 30.0f) {
			if (diff.x > 0.0f) {
				object3D_->worldTransform.rotate.z += -0.1f * angleRadians;
			}
			else {
				object3D_->worldTransform.rotate.z += 0.1f * angleRadians;
			}
		}
	}

	float attenuation = -0.5f * object3D_->worldTransform.rotate.z;
	if (std::fabs(attenuation) > std::fabs(object3D_->worldTransform.rotate.z)) {
		attenuation = -object3D_->worldTransform.rotate.z;
	}
	object3D_->worldTransform.rotate.z += attenuation;
}

void HeadPart::OnCollision(Collider* collider)
{
	bool isPart = collider->GetCollisionAttribute() == kCollisionAttributeDarumaPart;
	bool isTerrain = collider->GetCollisionAttribute() == kCollisionAttributeTerrain;
	bool isFoot = collider->GetCollisionAttribute() == kCollisionAttributeDarumaFoot;
	bool isCollision = (isPart || isTerrain);
	bool isTrue = collider->GetTag() == this->partTag_;
	if (isTerrain) {
		player_->SetIsDead(true);
	}
	if (isCollision && !isTrue) {
		CorrectPosition(collider);
		//AddTorque(collider);
		if (velocity_.y != 0.0f) {
			velocity_.y = 0.0f;
		}
	}
	object3D_->worldTransform.UpdateMatrix();
}

void HeadPart::ApplyGlobalVariables()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	std::string groupName = "HeadPart";
	object3D_->worldTransform.scale = global->GetVector3Value(groupName, "Scale");
	physics_.mass = global->GetFloatValue(groupName, "Mass");
	physics_.frictionCoeff = global->GetFloatValue(groupName, "FrictionCoeff");
	physics_.gravity = global->GetFloatValue(groupName, "Gravity");
	physics_.normalForce = physics_.mass * physics_.gravity;
}
