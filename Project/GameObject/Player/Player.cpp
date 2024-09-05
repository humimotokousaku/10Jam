#include "Player.h"
#include "../GameObjectLists.h"
#include "Input.h"

#include <cassert>

void Player::Initialize(Camera* camera)
{
	// チェック
	assert(camera);

	camera_ = camera;

}

void Player::Update()
{

	Input* input = Input::GetInstance();
	if (input->PressKey(DIK_A)) {
		for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
			if ((*it)->IsGround()) {
				(*it)->object3D_->worldTransform.translate.x -= 0.1f;
			}
		}
		//partsIt_ = parts_.begin();
		//if (partsIt_ != parts_.end()) {
		//	(*partsIt_)->object3D_->worldTransform.translate.x -= 0.1f;
		//}
	}
	else if (input->PressKey(DIK_D)) {
		for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
			if ((*it)->IsGround()) {
				(*it)->object3D_->worldTransform.translate.x += 0.1f;
			}
		}
		//partsIt_ = parts_.begin();
		//if (partsIt_ != parts_.end()) {
		//	(*partsIt_)->object3D_->worldTransform.translate.x += 0.1f;
		//}
	}
	if (input->PressKey(DIK_W)) {
		for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
			if ((*it)->IsGround()) {
				(*it)->object3D_->worldTransform.translate.z += 0.1f;
			}
		}
	}
	else if (input->PressKey(DIK_S)) {
		for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
			if ((*it)->IsGround()) {
				(*it)->object3D_->worldTransform.translate.z -= 0.1f;
			}
		}
	}

	if (input->TriggerKey(DIK_SPACE)) {
		partsIt_ = parts_.begin();
		if (partsIt_ != parts_.end()) {
			(*partsIt_)->velocity_ += pushPower_;
			//(*partsIt_)->object3D_->worldTransform.translate += pushPower_;
		}
	}

	for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
		(*it)->Update();
	}

	parts_.erase(std::remove_if(parts_.begin(), parts_.end(), [](const std::unique_ptr<IPart>& obj) {
		return obj->IsDead();
		}), parts_.end());
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

	ImGui::DragFloat3("PushPower", &pushPower_.x, 0.01f);

	// それぞれのImGui
	for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
		(*it)->ImGuiDraw();
	}

	ImGui::End();
}
