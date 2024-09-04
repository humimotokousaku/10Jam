#pragma once
#include "../../../Collision/Collider.h"

/// <summary>
/// プレイヤーの
/// </summary>
class IPart : public Collider
{
public:
	// 衝突時に呼ばれる関数
	void OnCollision(Collider* collider) override{ collider; }
	// ワールド座標を取得
	Vector3 GetWorldPosition() override{};
	// 角度を取得
	Vector3 GetRotation() override{};
public:
	virtual void Initialize() {};
	virtual void Update() {};
	virtual void Draw() {};

};
