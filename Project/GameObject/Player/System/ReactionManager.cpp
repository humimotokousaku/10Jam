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
	int index = 0;
	for (std::vector<std::unique_ptr<IPart>>::iterator it = partManager_->parts_.begin();
		it != partManager_->parts_.end(); ++it) {
		if ((*it)->IsTerrain()) {
			if ((*it)->GetTag() == "Head") {
				continue;
			}
			(*it)->velocity_ += moveDirect;
			//(*it)->object3D_->worldTransform.translate += moveDirect;
		}
		else {
			if ((*it)->GetTag() == "Head") {
				continue;
			}
			(*it)->velocity_ += moveDirect * 0.15f;
		}
	}

	//partManager_->partsIt_ = partManager_->parts_.begin();
	//if (partManager_->partsIt_ != partManager_->parts_.end()) {
	//	if ((*partManager_->partsIt_)->GetTag() == "Head") {
	//		return;
	//	}
	//	(*partManager_->partsIt_)->velocity_ += moveDirect;
	//	(*partManager_->partsIt_)->object3D_->worldTransform.translate += moveDirect;
	//}
}
