#pragma once
#include "Object3d.h"
#include "Plane.h"
#include "PostEffectManager.h"

class AttackDirection {
public:
	AttackDirection(Camera* camera);
	~AttackDirection() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	
#pragma region Setter
	// 矢印の方向ベクトルを設定
	void SetArrowDirection(Vector3 dirVel) { dirVel_ = dirVel; }
	// 矢印の描画開始
	void Start() { isStart_ = true; }
	// 矢印の描画終了
	void End() { isStart_ = false; }
#pragma endregion

#pragma region Getter
	// 矢印の方向ベクトルを取得
	Vector3 GetArrowDirection() { return dirVel_; }
#pragma endregion

public:
	// 全ての矢印のワールドトランスフォーム
	WorldTransform worldTransform_;

private:// エンジン機能
	PostEffectManager* postEffectManager_;
	ModelManager* modelManager_;
	TextureManager* textureManager_;

private:
	// カメラ
	Camera* camera_;

	// 矢印のスプライト
	//std::unique_ptr<Object3D> arrow_;
	std::unique_ptr<Plane> arrow_;

	// 使用するモデル
	Model* model_;
	// 矢印のテクスチャ番号
	uint32_t arrowTex_;

	// 方向ベクトル
	Vector3 dirVel_;

	// 矢印の表示開始
	bool isStart_;
};