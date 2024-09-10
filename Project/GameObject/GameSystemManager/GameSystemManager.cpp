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

	// 攻撃方向表示
	attackDirection_ = std::make_unique<AttackDirection>(player_->GetCamera());
	attackDirection_->Start();
	// 経過時間
	gameTimer_.Initialize();
}

void GameSystemManager::Update()
{
	// ゲームのタイマー
	gameTimer_.Update();
	gameTimer_.SetDrawTime(GetElapsedTime());
	// ゲーム終了の判定
	if (timer_.elapsed >= 60) {
		isGameEnd_ = true;
	}
	if (player_->IsDead()) {
		isGameEnd_ = true;
	}

	// ゲーム停止中
	if (timer_.isStop) {
		return;
	}
	// 時間の更新
	timer_.Update();

	// 攻撃方向表示
	attackDirection_->Update();
	attackDirection_->SetArrowDirection(actionDirect_);

	actionTime.coolTime.current++;
	if (actionTime.coolTime.current > actionTime.coolTime.max) {
		Action(actionPower_);
		actionTime.coolTime.current = 0;
	}
}

void GameSystemManager::ImGuiDraw()
{
	int time = this->timer_.elapsed;
	ImGui::InputInt("GameTime", &time);
	ImGui::DragFloat3("ActionDirect", &actionDirect_.x, 0.01f);
	ImGui::DragFloat("ActionPower", &actionPower_, 0.01f);
	if (ImGui::Button("PushAction")) {
		Action(actionPower_);
	}
	ImGui::Checkbox("GameStop", &isGameStop_);
	actionDirect_ = Normalize(actionDirect_);
}

void GameSystemManager::Draw()
{
	// 攻撃方向表示
	attackDirection_->Draw();
}

void GameSystemManager::Action(float power)
{
	// ここで力とアニメーションのセットアップ
	player_->GetReactionManager()->PushAction(actionDirect_, power);

}
