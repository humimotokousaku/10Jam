#pragma once
#include "Camera.h"

class FollowCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(bool isCountDown);

	// 追従対象からのオフセットを計算する
	Vector3 TargetOffset() const;

	///
	/// User Method
	/// 

	/// <summary>
	/// ゲームオーバ時のカメラ演出
	/// </summary>
	void GameOverAngle(const WorldTransform* worldTransform, Vector3 rotate) {
		isGameOver_ = true;
		gameOverAngle_ = rotate;
		destinationAngle_ = Vector2{ gameOverAngle_.x * 2, gameOverAngle_.y * 2 };
		camera_->worldTransform_.parent_ = worldTransform;
		offset_ = { 0,0,-10 };
	}

#pragma region Setter
	// 親子関係を設定
	void SetParent(const WorldTransform* target) {
		camera_->worldTransform_.parent_ = target;
	}
#pragma endregion

#pragma region Getter
	// ゲームオーバ演出中か
	bool CheckGameOverEffect() {
		if (gameOverTime_ <= 60) {
			return true;
		}
		return false;
	}

#pragma region Getter
	/// <summary>
	/// カメラのアドレスを取得
	/// </summary>
	/// <returns></returns>
	Camera* GetCamera() { return camera_.get(); }

	// カメラのワールド座標を取得
	const WorldTransform& GetWorldTransform() { return camera_->worldTransform_; }

	/// <summary>
	/// カメラのワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	Vector3 GetWorldPosition() {
		Vector3 result = {
			camera_->worldTransform_.matWorld_.m[3][0],
			camera_->worldTransform_.matWorld_.m[3][1],
			camera_->worldTransform_.matWorld_.m[3][2]
		};
		return result;
	}
#pragma endregion

	void RotateReset() {
		destinationAngle_ = {};
		camera_->worldTransform_.rotate.x = destinationAngle_.x;
		camera_->worldTransform_.rotate.y = destinationAngle_.y;
		camera_->Update();
	}

public:
	// カメラ補間の精度
	const float followRate = 0.1f;

private:
	// カメラ
	std::unique_ptr<Camera> camera_;

	// 後追い座標
	Vector3 interTarget_;

	Vector2 destinationAngle_;

	// カメラの傾き
	Vector2 tilt_;

	Vector3 offset_ = { 0,0,-70 };

	Vector3 gameOverAngle_;
	int gameOverTime_;
	bool isGameOver_;
};