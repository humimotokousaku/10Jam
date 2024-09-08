#include "ReactionManager.h"
#include "GameObjectLists.h"

#include <cassert>

void PlayerContext::ReactionManager::Initialize(Player* player)
{
	assert(player);
	player_ = player;
	partManager_ = player_->GetPartManager();

}

void PlayerContext::ReactionManager::PushAction(const Vector3& direct)
{
	Vector3 moveDirect = Normalize(direct);
	partManager_->partsIt_ = partManager_->parts_.begin();
	if (partManager_->partsIt_ != partManager_->parts_.end()) {
		(*partManager_->partsIt_)->velocity_ += moveDirect;
		(*partManager_->partsIt_)->object3D_->worldTransform.translate += moveDirect;
	}
}

void PlayerContext::ReactionManager::PushAction(const Vector3& direct, float power)
{
	Vector3 moveDirect = Normalize(direct);
	moveDirect *= power;
	partManager_->partsIt_ = partManager_->parts_.begin();
	if (partManager_->partsIt_ != partManager_->parts_.end()) {
		(*partManager_->partsIt_)->velocity_ += moveDirect;
		(*partManager_->partsIt_)->object3D_->worldTransform.translate += moveDirect;
	}
}
