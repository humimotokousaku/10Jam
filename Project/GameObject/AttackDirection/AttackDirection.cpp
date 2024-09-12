#include "AttackDirection.h"

AttackDirection::AttackDirection(Camera* camera) {
	camera_ = camera;
	Initialize();
}

void AttackDirection::Initialize() {
	postEffectManager_ = PostEffectManager::GetInstance();
	modelManager_ = ModelManager::GetInstance();
	textureManager_ = TextureManager::GetInstance();

	// 矢印テクスチャの読み込み
	textureManager_->LoadTexture("Textures", "arrow.png");
	arrowTex_ = textureManager_->GetSrvIndex("Textures", "arrow.png");

	// 矢印を張り付けるモデル
	modelManager_->LoadModel("Models/SampleBlock", "cube.obj");
	model_ = modelManager_->SetModel("Models/SampleBlock", "cube.obj");

	// 初期化
	worldTransform_.Initialize();
	worldTransform_.translate = { 0,0,0 };
	worldTransform_.scale = { 20.0f,0,20.0f };
	worldTransform_.UpdateMatrix();
	// 矢印のスプライトを生成
	for (int i = 0; i < arrow_.size(); i++) {
		arrow_[i] = std::make_unique<Plane>();
		arrow_[i]->Initialize();
		arrow_[i]->SetCamera(camera_);
		arrow_[i]->SetColor(Vector4{ 1,1,1,0.6f });
		arrow_[i]->worldTransform.parent_ = &worldTransform_;
	}

	// 大きさ
	afterImageAnim_[0].SetAnimData(&arrow_[1]->worldTransform.scale, Vector3{ 1,1,1 }, Vector3{ 1.5f,0.0f,1.5f }, 60, "s", Easings::EaseOutExpo);
	// 透明度
	afterImageAnim_[1].SetAnimData(arrow_[1]->GetColorP(), Vector4{1,1,1,1}, Vector4{1.0f,1.0f,1.0f, 0.0f}, 60, "s", Easings::EaseOutExpo);

	// 矢印の方向ベクトル
	dirVel_ = { 0,0,0 };

	// 矢印の表示開始
	isStart_ = false;
}

void AttackDirection::Update() {
#ifdef _DEBUG
	ImGui::Begin("AttackDirection");
	ImGui::DragFloat3("translation", &arrow_[0]->worldTransform.translate.x, 0.1f);
	ImGui::DragFloat3("WorldTranslate", &worldTransform_.translate.x, 0.1f);
	ImGui::DragFloat3("rotation", &arrow_[0]->worldTransform.rotate.x, 0.1f);
	ImGui::DragFloat3("scale", &arrow_[0]->worldTransform.scale.x, 0.1f);
	ImGui::DragFloat3("DirVel", &dirVel_.x, 0.1f);
	arrow_[0]->ImGuiAdjustParameter();
	ImGui::End();

#endif // _DEBUG
	// 表示していないなら処理しない
	if (!isStart_) { return; }

	// 方向ベクトルを正規化
	dirVel_ = Normalize(dirVel_);

	// 矢印の方向を算出
	for (int i = 0; i < arrow_.size(); i++) {
		// Y軸周り角度(θy)
		arrow_[i]->worldTransform.rotate.y = std::atan2(dirVel_.x, dirVel_.z);
		// 横軸方向の長さを求める
		float velocityXY;
		velocityXY = sqrt(dirVel_.x * dirVel_.x + dirVel_.y * dirVel_.y);
		// X軸周りの角度(θx)
		arrow_[i]->worldTransform.rotate.z = std::atan2(-dirVel_.y, velocityXY);
		// 行列を更新
		arrow_[i]->worldTransform.UpdateMatrix();
	}
	// 更新
	worldTransform_.UpdateMatrix();

#pragma region 残像の処理
	// アニメーションの更新
	for (int i = 0; i < afterImageAnim_.size(); i++) {
		afterImageAnim_[i].Update();
	}
	
	// 透明になったら終了
	if (afterImageAnim_[1].GetIsEnd()) {
		for (int i = 0; i < afterImageAnim_.size(); i++) {
			afterImageAnim_[i].SetIsStart(false);
		}
	}
#pragma endregion
}

void AttackDirection::Draw() {
	for (int i = 0; i < arrow_.size(); i++) {
		arrow_[i]->Draw(arrowTex_);
	}
}
