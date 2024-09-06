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

}

void Player::Update()
{

	Input* input = Input::GetInstance();
	float contIndex = 1.0f;

	if (input->PressKey(DIK_A)) {
		//partsIt_ = parts_.begin();
		//if (partsIt_ != parts_.end()) {
		//	(*partsIt_)->velocity_.x = -(0.1f);
		//}
		for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
			if ((*it)->IsGround()) {
				(*it)->velocity_.x = -(0.1f / contIndex);
			}
			contIndex++;
		}
	}
	else if (input->PressKey(DIK_D)) {
		for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
			if ((*it)->IsGround()) {
				(*it)->velocity_.x = (0.1f / contIndex);
			}
			contIndex++;
		}
	}
	else if (input->PressKey(DIK_W)) {
		for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
			if ((*it)->IsGround()) {
				(*it)->velocity_.z = (0.1f / contIndex);
			}
			contIndex++;
		}
	}
	else if (input->PressKey(DIK_S)) {
		for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
			if ((*it)->IsGround()) {
				(*it)->velocity_.z = -(0.1f / contIndex);
			}
			contIndex++;
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
		if (it == parts_.begin()) {
			(*it)->index_ = 0;
		}
		else
		{
			(*it)->index_ = 1;
		}
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

	if (ImGui::BeginTabBar("System"))
	{
		if (ImGui::BeginTabItem("PartManager")) {
			partManager_.ImGuiDraw();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::DragFloat3("PushPower", &pushPower_.x, 0.01f);

	if (ImGui::TreeNode("Parts")) {
		// それぞれのImGui
		for (std::vector<std::unique_ptr<IPart>>::iterator it = parts_.begin(); it != parts_.end(); ++it) {
			(*it)->ImGuiDraw();
		}
		ImGui::TreePop();
	}
	
	ImGui::End();
}
