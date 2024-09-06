#include "GameSystemManager.h"
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
}

void GameSystemManager::Update()
{
	// ゲーム停止中
	if (timer_.isStop) {
		return;
	}

	timer_.Update();

}

void GameSystemManager::Action()
{
	// ここで力とアニメーションのセットアップ


}
