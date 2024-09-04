#include "Player.h"
#include "../GameObjectLists.h"
#include "ModelManager.h"

#include <cassert>

void Player::Initialize(Camera* camera)
{
	// チェック
	assert(camera);

	camera_ = camera;

	testPart_ = std::make_unique<LightPart>();
	testPart_->Initialize(collisionManager_);
	testPart_->SetCamera(camera);
	testPart_->SetModel(ModelManager::GetInstance()->FindModel("", "cube.obj"));

}

void Player::Update()
{
	testPart_->Update();
}

void Player::Draw()
{
	testPart_->Draw();
}
