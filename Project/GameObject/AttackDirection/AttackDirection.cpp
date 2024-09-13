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
	audio_ = Audio::GetInstance();

	SE_[0] = audio_->SoundLoadWave("Music/SE/attack.wav");
	SE_[1] = audio_->SoundLoadWave("Music/SE/afterImage.wav");
	SE_[2] = audio_->SoundLoadWave("Music/SE/arrowPulsation.wav");

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
	for (int i = 0; i < arrow_.size(); i++) {
		arrow_[i] = std::make_unique<Plane>();
		arrow_[i]->Initialize();
		arrow_[i]->SetCamera(camera_);
		arrow_[i]->SetColor(Vector4{ 1,1,1,0.6f });
		arrow_[i]->worldTransform.parent_ = &worldTransform_;
	}

	// 大きさ
	afterImageAnim_[0].SetAnimData(&arrow_[1]->worldTransform.scale, Vector3{ 1,1,1 }, Vector3{ 1.5f,0.0f,1.5f }, 30, "s", Easings::EaseOutExpo);
	// 透明度
	afterImageAnim_[1].SetAnimData(arrow_[1]->GetColorP(), Vector4{ 1,1,1,1 }, Vector4{ 1.0f,1.0f,1.0f, 0.0f }, 30, "s", Easings::EaseOutExpo);

	// 30フレームごとに伸縮する
	afterImageAnim_[2].SetAnimData(&arrow_[0]->worldTransform.scale, Vector3{ 1,1,1 }, Vector3{ 0.85f,0.0f,0.85f }, 5, "s", Easings::EaseInOutSine);
	afterImageAnim_[3].SetAnimData(&arrow_[1]->worldTransform.scale, Vector3{ 1,1,1 }, Vector3{ 0.85f,0.0f,0.85f }, 5, "s", Easings::EaseInOutSine);

	// 矢印の方向ベクトル
	dirVel_ = { 0,0,0 };

	// 矢印の表示開始
	isStart_ = false;

	// 残像表示開始
	isAfterImage_ = false;

	isPulsation_ = false;

	pulsationCount_ = 0;

	currentFrame_ = 0;

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

#pragma region 3回伸縮する
	if (isPulsation_) {
		if (pulsationCount_ < 4) {
			if (currentFrame_ >= 30) {
				afterImageAnim_[2].ResetData();
				afterImageAnim_[3].ResetData();
				afterImageAnim_[2].SetIsStart(true);
				afterImageAnim_[3].SetIsStart(true);
				currentFrame_ = 0;
				pulsationCount_++;
				// 3回伸縮したら残像開始
				if (pulsationCount_ == 3) {
					isPulsation_ = false;
					isAfterImage_ = true;
					currentFrame_ = 0;
					pulsationCount_ = 0;
					afterImageAnim_[2].ResetData();
					afterImageAnim_[3].ResetData();

					afterImageAnim_[0].SetIsStart(true);
					afterImageAnim_[1].SetIsStart(true);
					audio_->SoundPlayWave(SE_[1], false, 0.1f);
				}
				
			}
			// アニメーション開始時に音を鳴らす
			// elseの影響で処理が1フレ遅れているので1フレの時に鳴らしている
			else if (currentFrame_ == 1) {
				audio_->SoundPlayWave(SE_[2], false, 0.4f);
			}
		}
		// アニメーションの更新
		for (int i = 2; i < afterImageAnim_.size(); i++) {
			afterImageAnim_[i].Update();
		}

		currentFrame_++;
	}
#pragma endregion
#pragma region 残像の処理
	if (isAfterImage_) {
		// アニメーションの更新
		for (int i = 0; i < afterImageAnim_.size() - 2; i++) {
			afterImageAnim_[i].Update();
		}

		// 残像が透明になったら終了
		if (afterImageAnim_[1].GetIsEnd()) {
			audio_->SoundPlayWave(SE_[0], false, 0.5f);
			isAfterImage_ = false;
			for (int i = 0; i < afterImageAnim_.size(); i++) {
				afterImageAnim_[i].ResetData();
			}
		}
	}
#pragma endregion
}

void AttackDirection::Draw() {
	for (int i = 0; i < arrow_.size(); i++) {
		arrow_[i]->Draw(arrowTex_);
	}
}

void AttackDirection::ApplyGlobalVariables()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	worldTransform_.translate.y = global->GetFloatValue("DirectInfo", "PositionY");
	worldTransform_.scale = global->GetVector3Value("DirectInfo", "Scale");

}
