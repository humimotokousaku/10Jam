#include "Player.h"
#include "../GameObjectLists.h"

#include <cassert>

void Player::Initialize(Camera* camera)
{
	// チェック
	assert(camera);

	camera_ = camera;

	testPart_ = std::make_unique<LightPart>();
	testPart_->Initialize(collisionManager_);
	testPart_->SetCamera(camera_);
	testPart_->SetModel(ModelManager::GetInstance()->FindModel("Models/SampleBlock", "cube.obj"));

}

void Player::Update()
{
	testPart_->Update();
	for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
		(*it)->Update();
	}
}

void Player::Draw()
{
	testPart_->Draw();

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
	ImGui::End();
	testPart_->ImGuiDraw();
}
