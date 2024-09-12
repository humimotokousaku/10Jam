#pragma once
#include "Object3D.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "Player/System/PhysicsSystem.h"
#include "GameSystemManager/GameSystemManager.h"

#include "Foot/FootCollider.h"

#include <memory>

class Player;

struct PartParameter {
	float gravity;
};

/// <summary>
/// プレイヤーの
/// </summary>
class IPart : public Collider
{
public:
	static uint32_t sSerialNumber;
public:
	uint32_t serialNumber_;
	std::string partTag_;
public: // 仮想関数
	// デストラクタ
	virtual ~IPart();
	virtual void Initialize(CollisionManager* manager);
	virtual void Update();
	virtual void Draw() = 0;
	virtual void ImGuiDraw();
	virtual void ApplyGlobalVariables();
protected: // 継承先で使用する関数
	void FootInitialize(CollisionManager* manager);
	void ColliderUpdate();
	void CorrectPosition(Collider* collider);

public: // アクセッサ
	// モデル
	void SetModel(Model* model) { object3D_->SetModel(model); }
	// カメラ
	void SetCamera(Camera* camera) { object3D_->SetCamera(camera); }
	// 接地
	void SetIsGround(bool isGround) { isGround_ = isGround; }
	// 死
	void SetIsDead(bool isDead) { isDead_ = isDead; }
	// 座標
	void SetPosition(const Vector3& position) {
		object3D_->worldTransform.translate = position;
		object3D_->worldTransform.UpdateMatrix();
	}

	void SetPlayer(Player* player) { player_ = player; }
public: // ゲッター
	WorldTransform* GetWorldTransform() { return &object3D_->worldTransform; }
	FootCollider* GetFootCollider() { return footCollider_.get(); }
	bool IsGround() { return isGround_; }
	bool IsDead() { return isDead_; }
public: // コライダー
	// 衝突時に呼ばれる関数
	// ワールド座標を取得
	Vector3 GetWorldPosition() override { return Vector3(object3D_->worldTransform.matWorld_.m[3][0], object3D_->worldTransform.matWorld_.m[3][1], object3D_->worldTransform.matWorld_.m[3][2]); };
	// 角度を取得
	Vector3 GetRotation() override { return Vector3(object3D_->worldTransform.rotate); };
	void OnCollision(Collider* collider) override;

protected:
	// 接地フラグ
	bool isGround_;
	// 消すフラグ
	bool isDead_;
	// 足場
	std::unique_ptr<FootCollider> footCollider_;
	// 物理のパラメータ
	PlayerContext::PhysicsParam physics_;
	// プレイヤー
	Player* player_ = nullptr;
private:
	// コライダーのマネージャ
	CollisionManager* collisionManager_;
public:
	// モデルと座標
	std::unique_ptr<Object3D> object3D_;
	// 速さ
	Vector3 velocity_;
	uint32_t index_ = 0;

private:
	// 一番下はTerrainとPart
	// 中はOverHeadとPart
	// Terrainのみの場合削除
	// 頭は例外
	struct SortStatus {
		bool isActive = false;
		int32_t activeCount = 0;
		int32_t maxCount = 25;
		void Initialize() { 
			isActive = false;
			activeCount = 0;
		}
		void Update() {
			activeCount++;
			if (activeCount > maxCount) {
				isActive = true;
			}
		}
	};

	struct RemoveStatus {
		bool isTerrain = false;	// 地形接地
		bool isOverHead = false;	// 頭上になにかいる
		bool isPart = false;	// 身体と接地
		int32_t deleteCount_;	// 削除までのカウント
		bool isDelete = false;	// 削除フラグ
		// 消すタイマーの処理を行うかどうか
		bool IsReadyCountDown() {
			bool isGame = isTerrain && (!isOverHead && !isPart);
			return (isGame);
		}
		bool IsAllFalse() {
			return !isTerrain && !isOverHead && !isPart;
		}
		// 値の全てをリセット
		void Initialize() {
			*this = RemoveStatus();
		}
		// フラグのリセット
		void FlagReset() {
			isTerrain = false;
			isOverHead = false;
			isPart = false;
		}
		void Update(int32_t max) {
			// フラグがtrueなら消すカウントダウン
			if (IsReadyCountDown()) {
				deleteCount_++;
				// 消すフラグを上げる
				if (deleteCount_ > max) {
					isDelete = true;
				}
			}
			// 削除フラグがなければ
			else {
				Initialize();
			}
		}
	};
	// 削除用の変数
	RemoveStatus removeStatus_;
	SortStatus sortStatus_;
};
