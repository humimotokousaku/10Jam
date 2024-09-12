#include "AttackDirection.h"
#include "GlobalVariables.h"

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
	worldTransform_.translate = { 0,-16.8f,0 };
	worldTransform_.scale = { 26.0f,0,26.0f };
	worldTransform_.UpdateMatrix();
	// 矢印のスプライトを生成
	arrow_ = std::make_unique<Plane>();
	arrow_->Initialize();
	arrow_->SetCamera(camera_);
	arrow_->SetColor(Vector4{ 1,1,1,0.6f });
	arrow_->worldTransform.parent_ = &worldTransform_;

	// 矢印の方向ベクトル
	dirVel_ = { 0,0,0 };

	// 矢印の表示開始
	isStart_ = false;

#ifdef _DEBUG

	GlobalVariables* global = GlobalVariables::GetInstance();
	global->CreateGroup("DirectInfo");
	global->AddItem("DirectInfo", "PositionY", worldTransform_.translate.y);
	global->AddItem("DirectInfo", "Scale", worldTransform_.scale);

#endif // _DEBUG
	ApplyGlobalVariables();

}

void AttackDirection::Update() {
#ifdef _DEBUG
	ImGui::Begin("AttackDirection");
	ImGui::DragFloat3("translation", &arrow_->worldTransform.translate.x, 0.1f);
	ImGui::DragFloat3("WorldTranslate", &worldTransform_.translate.x, 0.1f);
	ImGui::DragFloat3("rotation", &arrow_->worldTransform.rotate.x, 0.1f);
	ImGui::DragFloat3("scale", &arrow_->worldTransform.scale.x, 0.1f);
	ImGui::DragFloat3("DirVel", &dirVel_.x, 0.1f);
	arrow_->ImGuiAdjustParameter();
	ImGui::End();

#endif // _DEBUG
	// 表示していないなら処理しない
	if (!isStart_) { return; }

	// 方向ベクトルを正規化
	dirVel_ = Normalize(dirVel_);

	// 矢印の方向を算出
	// Y軸周り角度(θy)
	arrow_->worldTransform.rotate.y = std::atan2(dirVel_.x, dirVel_.z);
	// 横軸方向の長さを求める
	float velocityXY;
	velocityXY = sqrt(dirVel_.x * dirVel_.x + dirVel_.y * dirVel_.y);
	// X軸周りの角度(θx)
	arrow_->worldTransform.rotate.z = std::atan2(-dirVel_.y, velocityXY);
	// 行列を更新
	arrow_->worldTransform.UpdateMatrix();

	// 更新
	worldTransform_.UpdateMatrix();
}

void AttackDirection::Draw() {
	arrow_->Draw(arrowTex_);
}

void AttackDirection::ApplyGlobalVariables()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	worldTransform_.translate.y = global->GetFloatValue("DirectInfo", "PositionY");
	worldTransform_.scale = global->GetVector3Value("DirectInfo", "Scale");

}
