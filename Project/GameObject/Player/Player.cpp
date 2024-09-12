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

void Player::Update(bool isCountDown)
{
	if (!isCountDown) {
		partManager_.InputUpdate();
	}

	partManager_.Update();
}

void Player::Draw()
{
	partManager_.Draw();
}

void Player::ImGuiDraw()
{
	ImGui::Begin("Player");
	ImGui::Text("PlDead : %d", isDead_);
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

void Player::GenerateInitialize()
{
	// 死亡フラグリセット
	isDead_ = false;
	// リストの初期化
	partManager_.ListClear();
	// 生成
	partManager_.AddDaruma(PlayerContext::DarumaPattern::kL2M2H);
	partManager_.AddHead(Vector3{ 0,50,0 });
}

void Player::SolveConstraints()
{
	partManager_.Pushback();
}
