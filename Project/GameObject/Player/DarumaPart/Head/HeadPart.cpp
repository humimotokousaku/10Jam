#include "HeadPart.h"
#include "TextureManager.h"
#include "GlobalVariables.h"

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
	global->CreateGroup("HeadPart");
	Vector3 value = { 2.5f,2.5f,2.5f };
	global->AddItem("HeadPart", "Scale", value);
	object3D_->worldTransform.scale = global->GetVector3Value("HeadPart", "Scale");
	SetOBBLength(object3D_->worldTransform.scale);
	SetTag(partTag_);
	FootInitialize(manager);

	// USER
	isGround_ = false;
	// 物理
	physics_.mass = 10.0f;
	physics_.frictionCoeff = 0.3f;
	physics_.gravity = 9.8f;
	physics_.normalForce = physics_.mass * physics_.gravity;

	texture_ = TextureManager::GetInstance()->GetSrvIndex("", "uvChecker.png");
}

void HeadPart::Update()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	object3D_->worldTransform.scale = global->GetVector3Value("HeadPart", "Scale");
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
	if (isCollision && !isTrue) {
		CorrectPosition(collider);
		AddTorque(collider);
	}
	else if (isFoot) {
		if (!isTrue && index_ == 0) {
			isOtherFoot_ = true;
			return;
		}
	}
	if (isTerrain) {
		isTerrain_ = true;
	}
	object3D_->worldTransform.UpdateMatrix();
}

void HeadPart::ApplyGlobalVariables()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	object3D_->worldTransform.scale = global->GetVector3Value("HeadPart", "Scale");
}
