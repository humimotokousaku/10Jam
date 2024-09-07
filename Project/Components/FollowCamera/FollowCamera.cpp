#include "FollowCamera.h"
#include "Input.h"
#include "ImGuiManager.h"
#include <numbers>

void FollowCamera::Initialize() {
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
}

void FollowCamera::Update() {
	if (Input::GetInstance()->PressKey(DIK_RIGHT)) {
		destinationAngle_.y += 0.1f;
	}
	if (Input::GetInstance()->PressKey(DIK_LEFT)) {
		destinationAngle_.y -= 0.1f;
	}
	if (Input::GetInstance()->PressKey(DIK_UP)) {
		destinationAngle_.x -= 0.01f;
	}
	if (Input::GetInstance()->PressKey(DIK_DOWN)) {
		destinationAngle_.x += 0.01f;
	}

	// 追従対象からカメラまでのオフセット
	Vector3 offset = TargetOffset();
	// 追従座標の補間
	interTarget_ = Lerps::Lerp(interTarget_, camera_->worldTransform_.parent_->translate, 0.2f);
	// 座標をコピーしてオフセット分ずらす
	camera_->worldTransform_.translate = interTarget_ + offset;

	// x軸の回転制限
	destinationAngle_.x = std::clamp<float>(destinationAngle_.x, 0.03f, 0.8f);

	// カメラの補間
	camera_->worldTransform_.rotate.y =
		Lerps::LerpShortAngle(camera_->worldTransform_.rotate.y, destinationAngle_.y, followRate);
	camera_->worldTransform_.rotate.x =
		Lerps::LerpShortAngle(camera_->worldTransform_.rotate.x, destinationAngle_.x, followRate);

	// カメラの更新
	camera_->Update();

#ifdef _DEBUG
	ImGui::Begin("FollowCamera");
	ImGui::DragFloat3("translation", &camera_->worldTransform_.translate.x, 0.1f);
	ImGui::DragFloat3("rotation", &camera_->worldTransform_.rotate.x, 0.1f);
	ImGui::DragFloat2("rotationY", &destinationAngle_.x, 0.1f);
	ImGui::DragFloat("fov", &camera_->viewProjection_.fovAngleY, 0.1f, 0, 200);
	ImGui::End();
#endif // _DEBUG
}

Vector3 FollowCamera::TargetOffset() const {
	// 追従対象からのオフセット
	Vector3 offset = { 0, 0, -20 };
	// 回転行列を合成
	Matrix4x4 rotateMatrix = MakeRotateMatrix(camera_->worldTransform_.rotate);

	// オフセットをカメラの回転に合わせて回転
	offset = TransformNormal(offset, rotateMatrix);

	return offset;
}