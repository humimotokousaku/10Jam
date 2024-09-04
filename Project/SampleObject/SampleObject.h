#pragma once
// オブジェクト関係
#include "Object3D.h"
#include "Model.h"

// 当たり判定関係
#include "Collider.h"
#include "CollisionManager.h"

// カメラ
#include "Camera.h"

class SampleObject : public Collider {
public:
	~SampleObject();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	/// <summary>
	/// モデルを追加
	/// </summary>
	/// <param name="model">モデルデータ</param>
	void AddModel(Model* model) { models_.push_back(model); }

	/// <summary>
	/// カメラのアドレスを設定
	/// </summary>
	/// <param name="camera">カメラのアドレス</param>
	void SetCamera(Camera* camera) { camera_ = camera; }
	/// <summary>
	/// 衝突マネージャーのアドレスを設定
	/// </summary>
	/// <param name="collisionManager">衝突マネージャーのアドレス</param>
	void SetCollisionManager(CollisionManager* collisionManager) { collisionManager_ = collisionManager; }


	///
	/// Colliderの純粋仮想関数
	/// 

	// ワールド座標
	Vector3 GetWorldPosition() override;
	// 角度(オイラー角)
	Vector3 GetRotation() override;
	// 衝突判定
	void OnCollision(Collider* collider)override;


#pragma region 気にしなくていい関数
	void SetPosiotion(Vector3 pos) {
		object3d_->worldTransform.translate = pos;
		object3d_->worldTransform.UpdateMatrix();
	}

	void ImGuiParam(int index) {
		std::string beginName = "sampleBlock" + std::to_string(index);
		ImGui::Begin(beginName.c_str());
		ImGui::DragFloat3("translate", &object3d_->worldTransform.translate.x, 0.1f, -100.0f, 100.0f);
		ImGui::DragFloat3("rotate", &object3d_->worldTransform.rotate.x, 0.01f, -3.14f, 3.14f);
		ImGui::DragFloat3("scale", &object3d_->worldTransform.scale.x, 0.1f, -100.0f, 100.0f);
		ImGui::End();
	}
#pragma endregion

private:
	// 3Dオブジェクト
	std::unique_ptr<Object3D> object3d_;

	// 3Dオブジェクトに使用するモデル(3Dオブジェクトの見た目)
	std::vector<Model*> models_;

	// カメラのアドレス
	Camera* camera_;
	// 衝突マネージャのアドレス
	CollisionManager* collisionManager_;
};