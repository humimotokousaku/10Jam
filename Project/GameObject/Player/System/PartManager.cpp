#include "PartManager.h"
#include "../../GameObjectLists.h"
#include "ModelManager.h"

void PlayerContext::PartManager::Initialize(Player* player)
{
	player_ = player;
}

void PlayerContext::PartManager::Update()
{
	// 入力処理
	InputUpdate();

	// 更新
	for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
		if (it == parts_.begin()) {
			(*it)->index_ = 0;
		}
		else
		{
			(*it)->index_ = 1;
		}
		(*it)->Update();
	}

	// 削除処理
	parts_.erase(std::remove_if(parts_.begin(), parts_.end(), [this](const std::unique_ptr<IPart>& obj) {
		// 頭の死亡でプレイヤー全体の死亡判定
		if (obj->GetTag() == "Head" && obj->IsDead()) {
			player_->SetIsDead(true);
			return false;
		}
		return obj->IsDead();
		}), parts_.end());
}

void PlayerContext::PartManager::Draw()
{
	for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
		(*it)->Draw();
	}
}

void PlayerContext::PartManager::InputUpdate()
{
	Input* input = Input::GetInstance();
	Vector2 leftStick = {};
	XINPUT_STATE joyState;
	if (input->GetJoystickState(0, joyState)) {
		SHORT leftThumbX = input->ApplyDeadzone(joyState.Gamepad.sThumbLX);
		SHORT leftThumbY = input->ApplyDeadzone(joyState.Gamepad.sThumbLY);
		leftStick.x += (float)leftThumbX / SHRT_MAX;
		leftStick.y += (float)leftThumbY / SHRT_MAX;
		leftStick = Normalize(leftStick);
		leftStick *= 0.1f;
	}
	else {
		//float moveValue = 0.1f;
		//bool isA = false;
		//bool isD = false;
		//bool isW = false;
		//bool isS = false;
		//Vector2 moveSpeed = { moveValue,moveValue };

		//// 入力処理
		//if (input->PressKey(DIK_A)) {
		//	isA = true;
		//	moveSpeed.x *= -1.0f;
		//}
		//if (input->PressKey(DIK_D)) {
		//	isD = true;
		//}
		//if (input->PressKey(DIK_W)) {
		//	isW = true;
		//}
		//if (input->PressKey(DIK_S)) {
		//	isS = true;
		//	moveSpeed.y *= -1.0f;
		//}

		//// スピードの調整
		//if (isA || isD) {
		//	if (isW || isS) {
		//		moveSpeed.x *= (1.0f / 2.0f);
		//		moveSpeed.y *= (1.0f / 2.0f);
		//	}
		//	else {
		//		moveSpeed.y = 0.0f;
		//	}
		//}
		//else if (isW || isS) {
		//	if (isA || isD) {
		//		moveSpeed.x *= (1.0f / 2.0f);
		//		moveSpeed.y *= (1.0f / 2.0f);
		//	}
		//	else {
		//		moveSpeed.x = 0.0f;
		//	}
		//}
		//else {
		//	moveSpeed = { 0.0f,0.0f };
		//}
	}
	float contIndex = 1.0f;

	// 入力による速度の計算
	for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
		if ((*it)->IsGround()) {
			(*it)->velocity_.x += leftStick.x * (contIndex);
			(*it)->velocity_.z += leftStick.y * (contIndex);
		}
		contIndex += 0.05f;
	}
}

void PlayerContext::PartManager::AddHead(const Vector3& position)
{
	std::unique_ptr<IPart> instance = std::make_unique<HeadPart>();
	instance->Initialize(player_->GetCollision());
	instance->SetCamera(player_->GetCamera());
	instance->SetModel(ModelManager::GetInstance()->FindModel("Models/SampleBlock", "cube.obj"));
	instance->GetFootCollider()->Object3DSetting(player_->GetCamera(), ModelManager::GetInstance()->FindModel("Models/SampleBlock", "cube.obj"));
	instance->SetPosition(position);
	parts_.push_back(std::move(instance));
}

void PlayerContext::PartManager::AddDaruma(DarumaPattern pattern)
{
	int index = 0;
	switch (pattern)
	{
	case PlayerContext::DarumaPattern::kDefault:
		for (int i = 0; i < 3; i++) {
			AddParts<MediumPart>(Vector3(0.0f, float(i) * 3.85f, 0.0f));
		}
		//AddParts<MediumPart>(Vector3(0.0f, 2.0f, 0.0f));
		break;
	case PlayerContext::DarumaPattern::kHeavy:
		for (int i = 0; i < 3; i++) {
			AddParts<HeavyPart>(Vector3(0.0f, float(i) * 3.85f, 0.0f));
		}
		//AddParts<MediumPart>(Vector3(0.0f, 2.0f, 0.0f));
		break;
	case PlayerContext::DarumaPattern::kL2M2H:
		AddParts<LightPart>(Vector3(0.0f, float(index) * 3.85f, 0.0f));
		index++;
		AddParts<MediumPart>(Vector3(0.0f, float(index) * 3.85f, 0.0f));
		index++;
		AddParts<HeavyPart>(Vector3(0.0f, float(index) * 3.85f, 0.0f));
		break;
	case PlayerContext::DarumaPattern::kH2M2L:
		AddParts<HeavyPart>(Vector3(0.0f, float(index) * 3.85f, 0.0f));
		index++;
		AddParts<MediumPart>(Vector3(0.0f, float(index) * 3.85f, 0.0f));
		index++;
		AddParts<LightPart>(Vector3(0.0f, float(index) * 3.85f, 0.0f));
		break;
	default:
		break;
	}

}
void PlayerContext::PartManager::ImGuiDraw()
{
	float dragValue = 0.01f;
	ImGui::DragFloat3("GeneratePosition", &generatePosition_.x, dragValue);
	ImGui::Separator();
	if (ImGui::Button("AddLight")) {
		AddParts<LightPart>(generatePosition_);
	}
	if (ImGui::Button("Medium")) {
		AddParts<MediumPart>(generatePosition_);
	}
	if (ImGui::Button("AddHeavy")) {
		AddParts<HeavyPart>(generatePosition_);
	}
	if (ImGui::Button("AddHead")) {
		AddHead(generatePosition_);
	}
	ImGui::Separator();
	if (ImGui::TreeNode("SetTower")) {
		if (ImGui::Button("SetDefault")) {
			AddDaruma(DarumaPattern::kDefault);
		}
		if (ImGui::Button("SetHeavy")) {
			AddDaruma(DarumaPattern::kHeavy);
		}
		if (ImGui::Button("LightToHeavy")) {
			AddDaruma(DarumaPattern::kL2M2H);
		}
		if (ImGui::Button("HeavyToLight")) {
			AddDaruma(DarumaPattern::kH2M2L);
		}
		ImGui::TreePop();
	}
	if (ImGui::Button("Reset")) {
		parts_.clear();
	}
}

void PlayerContext::PartManager::ImGuiPart()
{
	// 更新
	for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
		(*it)->ImGuiDraw();
	}
}

// 明示的なテンプレートのインスタンス化
template void PlayerContext::PartManager::AddParts<LightPart>(const Vector3& position);
template void PlayerContext::PartManager::AddParts<HeavyPart>(const Vector3& position);
template void PlayerContext::PartManager::AddParts<MediumPart>(const Vector3& position);

template<typename T>
void PlayerContext::PartManager::AddParts(const Vector3& position)
{
	std::unique_ptr<IPart> instance = std::make_unique<T>();
	instance->Initialize(player_->GetCollision());
	instance->SetCamera(player_->GetCamera());
	instance->SetModel(ModelManager::GetInstance()->FindModel("Models/DarumaBody", "DarumaBody.obj"));
	instance->GetFootCollider()->Object3DSetting(player_->GetCamera(), ModelManager::GetInstance()->FindModel("Models/SampleBlock", "cube.obj"));
	instance->SetPosition(position);
	parts_.push_back(std::move(instance));
}
