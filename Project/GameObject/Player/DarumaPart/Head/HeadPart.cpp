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
	ApplyGlobalVariables();

	texture_ = TextureManager::GetInstance()->GetSrvIndex("", "uvChecker.png");
}

void HeadPart::Update()
{
	// グローバルうんたら
	ApplyGlobalVariables();

	// 落下処理
	if (!isGround_) {
		velocity_.y -= (4.5f * GameSystemManager::sDeltaTime);
	}
	else {
		velocity_.y = 0.0f;
	}

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
	//IPart::Update();
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
	float torque = 0.0f;
	// 当たったオブジェクトの最大最小
	Vector3 min = collider->GetWorldPosition() - collider->GetOBB().m_fLength;
	Vector3 max = collider->GetWorldPosition() + collider->GetOBB().m_fLength;

	Vector3 scope = GetOBB().m_fLength;
	float coefficient = 0.0f;

	// z軸回転
	if (GetWorldPosition().x < min.x) {
		coefficient = std::fabs(object3D_->worldTransform.translate.x - min.x) / scope.x;
		object3D_->worldTransform.rotate.z = Lerps::Lerp(0.0f, 45.0f * float(std::numbers::pi) / 180.0f, coefficient);
	}
	else if (GetWorldPosition().x > max.x) {
		coefficient = std::fabs(object3D_->worldTransform.translate.x - max.x) / scope.x;
		object3D_->worldTransform.rotate.z = Lerps::Lerp(0.0f, -45.0f * float(std::numbers::pi) / 180.0f, coefficient);
	}
	else {
		torque = -0.1f * object3D_->worldTransform.rotate.z;
		object3D_->worldTransform.rotate.z += torque;
	}
	// x軸回転
	if (GetWorldPosition().z < min.z) {
		coefficient = std::fabs(object3D_->worldTransform.translate.z - min.z) / scope.z;
		object3D_->worldTransform.rotate.x = Lerps::Lerp(0.0f, -45.0f * float(std::numbers::pi) / 180.0f, coefficient);
	}
	else if (GetWorldPosition().z > max.z) {
		coefficient = std::fabs(object3D_->worldTransform.translate.z - max.z) / scope.z;
		object3D_->worldTransform.rotate.x = Lerps::Lerp(0.0f, 45.0f * float(std::numbers::pi) / 180.0f, coefficient);
	}
	else {
		torque = -0.1f * object3D_->worldTransform.rotate.x;
		object3D_->worldTransform.rotate.x += torque;
	}
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
		AddTorque(collider);
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
