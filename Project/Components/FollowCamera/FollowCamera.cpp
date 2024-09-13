#include "FollowCamera.h"
#include "Input.h"
#include "ImGuiManager.h"
#include <numbers>

void FollowCamera::Initialize() {
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	tilt_ = { 0,0 };
	gameOverAngle_ = { 0,0,0 };
	isGameOver_ = false;
	gameOverTime_ = 0;
}

void FollowCamera::Update(bool isCountDown) {
	Vector2 rightStick{};
	XINPUT_STATE joyState{};
	Vector2 rot{};
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (!isCountDown) {
			SHORT rightThumbX = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbRX);
			SHORT rightThumbY = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbRY);
			rightStick.x -= (float)rightThumbY / SHRT_MAX;
			rightStick.y += (float)rightThumbX / SHRT_MAX;
			rightStick = Normalize(rightStick) * 0.8f;
			rot.x += rightStick.y * 0.05f;
			rot.y += rightStick.x * 0.05f;
		}
	}
	else {
		if (Input::GetInstance()->PressKey(DIK_RIGHT)) {
			rot.x -= 0.01f;
			destinationAngle_.y += 0.1f;
		}
		if (Input::GetInstance()->PressKey(DIK_LEFT)) {
			rot.x += 0.01f;
			destinationAngle_.y -= 0.1f;
		}
		if (Input::GetInstance()->PressKey(DIK_UP)) {
			destinationAngle_.x -= 0.01f;
		}
		if (Input::GetInstance()->PressKey(DIK_DOWN)) {
			destinationAngle_.x += 0.01f;
		}
	}
	destinationAngle_ += rightStick * 0.1f;

	// 徐々に速度を上げる,下げる
	tilt_.x = Lerps::ExponentialInterpolate(tilt_, rot, 1.0f, 0.05f).x;
	
	// x軸の回転制限
	destinationAngle_.x = std::clamp<float>(destinationAngle_.x, 0.03f, 0.8f);

	// カメラの補間
	camera_->worldTransform_.rotate.y =
		Lerps::LerpShortAngle(camera_->worldTransform_.rotate.y, destinationAngle_.y, followRate);
	camera_->worldTransform_.rotate.x =
		Lerps::LerpShortAngle(camera_->worldTransform_.rotate.x, destinationAngle_.x, followRate);
	camera_->worldTransform_.rotate.z = tilt_.x;

	// 追従対象からカメラまでのオフセット
	Vector3 offset = TargetOffset();
	// 追従座標の補間
	interTarget_ = Lerps::Lerp(interTarget_, camera_->worldTransform_.parent_->translate, 0.2f);
	// 座標をコピーしてオフセット分ずらす
	camera_->worldTransform_.translate = interTarget_ + offset;

	// カメラの更新
	camera_->Update();

	if (isGameOver_) {
		gameOverTime_++;
	}

#ifdef _DEBUG
	ImGui::Begin("FollowCamera");
	ImGui::DragFloat3("translation", &camera_->worldTransform_.translate.x, 0.1f);
	ImGui::DragFloat3("rotation", &camera_->worldTransform_.rotate.x, 0.1f);
	ImGui::DragFloat2("rotationY", &destinationAngle_.x, 0.1f);
	ImGui::DragFloat("fov", &camera_->viewProjection_.fovAngleY, 0.1f, 0, 200);
	ImGui::DragFloat3("offset", &offset_.x, 0.1f, -100, 100);
	ImGui::End();
#endif // _DEBUG
}

Vector3 FollowCamera::TargetOffset() const {
	// 追従対象からのオフセット
	Vector3 offset = offset_;
	// 回転行列を合成
	Vector3 rotate = camera_->worldTransform_.rotate;
	Matrix4x4 rotateMatrix = MakeRotateMatrix(rotate);

	// オフセットをカメラの回転に合わせて回転
	offset = TransformNormal(offset, rotateMatrix);

	return offset;
}