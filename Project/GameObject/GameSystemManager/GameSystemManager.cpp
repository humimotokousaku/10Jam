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

	timer_.elapsed = 5;
	timer_.frameCount = 0;

	// 方向
	actionDirect_ = { -1.0f,0.0f,1.0f };
	// 攻撃方向表示
	attackDirection_ = std::make_unique<AttackDirection>(player_->GetCamera());
	attackDirection_->Start();
	attackDirection_->worldTransform_.translate.y = -16.8f;
	attackDirection_->worldTransform_.scale *= 3.0f;
	attackDirection_->SetColor({ 1.0f,1.0f,1.0f,0.8f });
	// 経過時間
	gameTimer_.Initialize();
}

void GameSystemManager::Update()
{
	// ゲーム終了の判定
	if (timer_.elapsed <= 0) {
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
	timer_.Update(-1);
	// ゲームのタイマー
	gameTimer_.Update();
	gameTimer_.SetDrawTime(GetElapsedTime());

	// 攻撃方向表示
	attackDirection_->Update();
	attackDirection_->SetArrowDirection(actionDirect_);

	PushActionTimeReference();

	//actionTime.coolTime.current++;
	//if (actionTime.coolTime.current > actionTime.coolTime.max) {
	//	Action(actionPower_);
	//	actionTime.coolTime.current = 0;
	//}
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
	// アクションしたか
	timer_.isAction = true;
}

void GameSystemManager::PushActionTimeReference()
{
	// 押し出しのやつをタイマーで
	if (timer_.elapsed == 10) {
		if (timer_.isAction) {
			return;
		}
		Action(2.0f);
		actionDirect_ = { 1.0f,0.0f,1.0f };
	}
	else if (timer_.elapsed == 15) {
		if (timer_.isAction) {
			return;
		}
		Action(1.5f);
		actionDirect_ = { 1.0f,0.0f,-1.0f };
	}
	else if (timer_.elapsed == 25) {
		if (timer_.isAction) {
			return;
		}
		Action(1.0f);
		actionDirect_ = { -1.0f,0.0f,-1.0f };
	}
	else {
		timer_.isAction = false;
	}
}
