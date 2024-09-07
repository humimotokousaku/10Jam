#include "Player.h"
#include "../GameObjectLists.h"
#include "Input.h"

#include <cassert>

void Player::Initialize(Camera* camera)
{
	// チェック
	assert(camera);

	camera_ = camera;

	partManager_.Initialize(this);
	reactionManager_.Initialize(this);

}

void Player::Update()
{

	partManager_.Update();

}

void Player::Draw()
{
	partManager_.Draw();
}

void Player::ImGuiDraw()
{
	ImGui::Begin("Player");

	if (ImGui::BeginTabBar("System"))
	{
		if (ImGui::BeginTabItem("PartManager")) {
			partManager_.ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Parts")) {
			// それぞれのImGui
			partManager_.ImGuiPart();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
	
	ImGui::End();
}
