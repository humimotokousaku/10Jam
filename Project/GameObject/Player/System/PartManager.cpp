#include "PartManager.h"
#include "../../GameObjectLists.h"
#include "ModelManager.h"

void PlayerContext::PartManager::Initialize(Player* player)
{

	player_ = player;

}

void PlayerContext::PartManager::AddDaruma(DarumaPattern pattern)
{

	switch (pattern)
	{
	case PlayerContext::DarumaPattern::kDefault:
		for (int i = 0; i < 5; i++) {
			AddParts<MediumPart>(Vector3(0.0f, float(i) * 3.85f, 0.0f));
		}
		//AddParts<MediumPart>(Vector3(0.0f, 2.0f, 0.0f));
		break;
	default:
		break;
	}

}
void PlayerContext::PartManager::ImGuiDraw()
{
	float dragValue = 0.01f;
	ImGui::DragFloat3("GeneratePosition", &generatePosition_.x, dragValue);

	if (ImGui::Button("AddLight")) {
		AddParts<LightPart>(generatePosition_);
	}
	if (ImGui::Button("Medium")) {
		AddParts<MediumPart>(generatePosition_);
	}
	if (ImGui::Button("AddHeavy")) {
		AddParts<HeavyPart>(generatePosition_);
	}

	if (ImGui::Button("Set")) {
		AddDaruma(DarumaPattern::kDefault);
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
	instance->SetModel(ModelManager::GetInstance()->FindModel("Models/SampleBlock", "cube.obj"));
	instance->GetFootCollider()->Object3DSetting(player_->GetCamera(), ModelManager::GetInstance()->FindModel("Models/SampleBlock", "cube.obj"));
	instance->SetPosition(position);
	player_->parts_.push_back(std::move(instance));
}
