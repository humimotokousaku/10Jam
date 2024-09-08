#include "GameSystemManager.h"
#include "GameObjectLists.h"
#include <cassert>

void GameSystemManager::Initialize(Player* player, Enemy* enemy)
{
	// ポインタ
	assert(player);
	assert(enemy);
	player_ = player;
	enemy_ = enemy;

	timer_.elapsed = 0;
	timer_.frameCount = 0;

	actionTime.coolTime = { 0,300 };

}

void GameSystemManager::Update()
{
	// ゲーム停止中
	if (timer_.isStop) {
		return;
	}

	timer_.Update();

	actionTime.coolTime.current++;
	if (actionTime.coolTime.current > actionTime.coolTime.max) {
		Action(0.15f);
		actionTime.coolTime.current = 0;
	}

}

void GameSystemManager::Action(float power)
{
	// ここで力とアニメーションのセットアップ
	player_->GetReactionManager()->PushAction({ 1.0f,0.0f,1.0f }, power);

}
