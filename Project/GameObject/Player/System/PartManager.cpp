#include "PartManager.h"
#include "../../GameObjectLists.h"
#include "ModelManager.h"
#include "GlobalVariables.h"

void PlayerContext::PartManager::Initialize(Player* player)
{
	player_ = player;
#ifdef _DEBUG
	GlobalVariables* global = GlobalVariables::GetInstance();
	global->CreateGroup("InputInfo");
	global->AddItem("InputInfo", "InputRatio", 0.1f);
	global->AddItem("InputInfo", "IndexAddValue", 0.1f);
#endif // _DEBUG

}

void PlayerContext::PartManager::Update()
{
	// 更新
	listIndex_ = 0;
	for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
		(*it)->index_ = listIndex_;
		(*it)->Update();
		listIndex_++;
	}
	// ソート
	std::sort(parts_.begin(), parts_.end(),
		[](const std::unique_ptr<IPart>& a, const std::unique_ptr<IPart>& b) {
			return a->index_ < b->index_;
		});

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
	}
	else {
		if (input->PressKey(DIK_A)) {
			leftStick.x -= 1.0f;
		}
		else if (input->PressKey(DIK_D)) {
			leftStick.x += 1.0f;
		}
		if (input->PressKey(DIK_W)) {
			leftStick.y += 1.0f;
		}
		else if (input->PressKey(DIK_S)) {
			leftStick.y -= 1.0f;
		}
		leftStick = Normalize(leftStick);
	}
	float contIndex = 1.0f;

	// カメラの回転に合わせた方向に変換
	Matrix4x4 yawMatrix = MakeRotateYMatrix(player_->GetCamera()->worldTransform_.rotate.y);
	Vector3 rotateVector = TransformNormal({ leftStick.x,0.0f,leftStick.y }, yawMatrix);
	leftStick = { rotateVector.x,rotateVector.z };
	leftStick *= GlobalVariables::GetInstance()->GetFloatValue("InputInfo", "InputRatio");

	// 入力による速度の計算
	for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
		if ((*it)->IsGround()) {
			(*it)->velocity_.x += leftStick.x * (contIndex);
			(*it)->velocity_.z += leftStick.y * (contIndex);
		}
		contIndex += GlobalVariables::GetInstance()->GetFloatValue("InputInfo", "IndexAddValue");;
	}
}

void PlayerContext::PartManager::Pushback()
{
	for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
		(*it)->SetPosition((*it)->GetWorldPosition() + (*it)->totalPushback_);
		(*it)->totalPushback_ = { 0.0f, 0.0f, 0.0f };
		(*it)->pushback_ = { 0.0f, 0.0f, 0.0f };
	}
}

void PlayerContext::PartManager::AddHead(const Vector3& position)
{
	std::unique_ptr<IPart> instance = std::make_unique<HeadPart>();
	instance->Initialize(player_->GetCollision());
	instance->SetCamera(player_->GetCamera());
	instance->SetModel(ModelManager::GetInstance()->FindModel("Models/DarumaHead", "DarumaHead.obj"));
	instance->GetFootCollider()->Object3DSetting(player_->GetCamera(), ModelManager::GetInstance()->FindModel("Models/DarumaHead", "DarumaHead.obj"));
	instance->SetPosition(position);
	instance->SetPlayer(player_);
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
		break;
	case PlayerContext::DarumaPattern::kHeavy:
		for (int i = 0; i < 3; i++) {
			AddParts<HeavyPart>(Vector3(0.0f, float(i) * 3.85f, 0.0f));
		}
		break;
	case PlayerContext::DarumaPattern::kL2M2H:
		AddParts<LightPart>(Vector3(0.0f, float(index) * 3.0f, 0.0f));
		index++;
		//AddParts<LightPart>(Vector3(0.0f, float(index) * 3.0f, 0.0f));
		//index++;
		AddParts<MediumPart>(Vector3(0.0f, float(index) * 3.0f, 0.0f));
		index++;
		AddParts<LightPart>(Vector3(0.0f, float(index) * 3.0f, 0.0f));
		index++;
		AddParts<HeavyPart>(Vector3(0.0f, float(index) * 3.0f, 0.0f));
		break;
	case PlayerContext::DarumaPattern::kH2M2L:
		AddParts<HeavyPart>(Vector3(0.0f, float(index) * 3.85f, 0.0f));
		index++;
		AddParts<MediumPart>(Vector3(0.0f, float(index) * 3.85f, 0.0f));
		index++;
		AddParts<LightPart>(Vector3(0.0f, float(index) * 3.85f, 0.0f));
		index++;
		AddParts<MediumPart>(Vector3(0.0f, float(index) * 3.85f, 0.0f));
		break;
	default:
		break;
	}

}

void PlayerContext::PartManager::ListClear()
{
	parts_.clear();
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
	if (ImGui::Button("Sort")) {
		// 昇順に
		std::sort(parts_.begin(), parts_.end(),
			[](const std::unique_ptr<IPart>& a, const std::unique_ptr<IPart>& b) {
				return a->index_ < b->index_;
			});
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
		player_->SetIsDead(false);
		ListClear();
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
