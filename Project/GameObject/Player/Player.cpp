#include "Player.h"
#include "../GameObjectLists.h"

#include <cassert>

void Player::Initialize(Camera* camera)
{
	// チェック
	assert(camera);

	camera_ = camera;

}

void Player::Update()
{
	for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
		(*it)->Update();
	}
}

void Player::Draw()
{
	for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
		(*it)->Draw();
	}

}

void Player::ImGuiDraw()
{
	ImGui::Begin("Player");
	if (ImGui::Button("AddLight")) {
		AddParts<LightPart>();
	}
	if (ImGui::Button("AddHeavy")) {
		AddParts<HeavyPart>();
	}

	// それぞれのImGui
	for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
		(*it)->ImGuiDraw();
	}

	ImGui::End();
}
