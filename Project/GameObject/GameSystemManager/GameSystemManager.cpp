#include "GameSystemManager.h"
#include "GameObjectLists.h"
#include <cassert>

float GameSystemManager::sDeltaTime = 1.0f / 60.0f;

void GameSystemManager::Initialize(Player* player, Enemy* enemy)
{
	// ポインタ
	assert(player);
	assert(enemy);
	player_ = player;
	enemy_ = enemy;

	timer_.elapsed = 60;
	timer_.frameCount = 0;

	// 方向
	actionDirect_ = { -1.0f,0.0f,1.0f };
	// 攻撃方向表示
	attackDirection_ = std::make_unique<AttackDirection>(player_->GetCamera());
	attackDirection_->Start();
	attackDirection_->worldTransform_.translate.y = -16.8f;
	//attackDirection_->worldTransform_.scale *= 3.0f;
	attackDirection_->SetColor({ 1.0f,1.0f,1.0f,0.8f });
	// 経過時間
	gameTimer_.Initialize();
	gameTimer_.SetDrawTime(GetElapsedTime());

	// 外部の行動データ
	actionManager_ = std::make_unique<ActionManager>();
	actionManager_->LoadActionData();
	attackDirection_->SetArrowDirection(actionManager_->actionContainer_[0].direct);
}

void GameSystemManager::Update(bool isTutorial)
{
	// チュートリアル中なら処理しない
	if (isTutorial) {
		return;
	}
	// ゲーム終了の判定
	if (timer_.elapsed <= 0) {
		isGameEnd_ = true;
		isGameClear_ = true;
	}
	// 失敗の場合
	if (player_->IsDead()) {
		isGameEnd_ = true;
		isGameOver_ = true;
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

	// CSVのデータを使ったアクション処理
	CSVActionControll();
}

void GameSystemManager::ImGuiDraw()
{
	int time = this->timer_.elapsed;
	ImGui::InputInt("GameTime", &time);
	ImGui::DragFloat3("ActionDirect", &actionDirect_.x, 0.01f);
	ImGui::DragFloat("ActionPower", &actionPower_, 0.01f);
	if (ImGui::Button("PushAction")) {
		Action(actionDirect_, actionPower_);
	}
	ImGui::Checkbox("GameStop", &isGameStop_);
	actionDirect_ = Normalize(actionDirect_);
}

void GameSystemManager::Draw()
{
	// 攻撃方向表示
	attackDirection_->Draw();
}

void GameSystemManager::Action(const Vector3& direct, float power)
{
	// ここで力とアニメーションのセットアップ
	player_->GetReactionManager()->PushAction(direct, power);
	// アクションしたか
	timer_.isAction = true;
}

void GameSystemManager::CSVActionControll()
{
	// 配列を超過しないように
	if (actionNow_ >= actionManager_->actionContainer_.size()) {
		return;
	}
	else {
		float alpha = ((float)actionManager_->actionContainer_[actionNow_].time * 60.0f) / ((float)timer_.elapsed * 60.0f + (float)timer_.frameCount);
		attackDirection_->SetColor({ 1.0f,1.0f,1.0f,alpha });
	}
	// 画像の方向設定
	attackDirection_->SetArrowDirection(actionManager_->actionContainer_[actionNow_].direct);
	// 押し出すアクション
	if (actionManager_->actionContainer_[actionNow_].time == timer_.elapsed) {
		// 一度行っているなら早期
		if (timer_.isAction) {
			return;
		}
		Action(actionManager_->actionContainer_[actionNow_].direct, actionManager_->actionContainer_[actionNow_].power);
		actionNow_++;
	}
	// その番号の時間と一致していなければフラグリセット
	else {
		timer_.isAction = false;
	}
}
