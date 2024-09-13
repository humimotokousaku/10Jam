#pragma once
#include "Object3d.h"
#include "Plane.h"
#include "PostEffectManager.h"
#include "Animation.h"
#include "Audio.h"

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
	/// <summary>
	/// Globalの値を適応する処理
	/// </summary>
	void ApplyGlobalVariables();
	
#pragma region Setter
	// 矢印の方向ベクトルを設定
	void SetArrowDirection(Vector3 dirVel) { dirVel_ = dirVel; }
	void SetColor(const Vector4& color) { arrow_[0]->SetColor(color); }
	// 矢印の描画開始
	void Start() { isStart_ = true; }
	// 矢印の描画終了
	void End() { isStart_ = false; }
	// 矢印の残像を表示する
	void StartAfterImage() { 
		isPulsation_ = true;
		// 伸縮アニメーション開始
		afterImageAnim_[2].SetIsStart(true);
		afterImageAnim_[3].SetIsStart(true);
	}
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
	Audio* audio_;

private:
	// カメラ
	Camera* camera_;

	// 矢印のスプライト
	std::array<std::unique_ptr<Plane>, 2> arrow_;

	// 使用するモデル
	Model* model_;
	// 矢印のテクスチャ番号
	uint32_t arrowTex_;

	// SE
	std::array<uint32_t, 3> SE_;

	// 残像用のアニメーション
	std::array<Animation, 4> afterImageAnim_;

	// 方向ベクトル
	Vector3 dirVel_;

	// 矢印の表示開始
	bool isStart_;
	// 残像表示開始
	bool isAfterImage_ = false;
	bool isPulsation_ = false;

	// 伸縮する回数
	int pulsationCount_;

	int currentFrame_;
};