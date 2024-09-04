#pragma once
#include "../../Collision/Collider.h"

class IEnemy : public Collider
{
public:
	virtual void Initialize() {};
	virtual void Update() {};
	virtual void Draw() {};

public:
	// 衝突時に呼ばれる関数
	void OnCollision(Collider* collider) override{ collider; }
	// ワールド座標を取得
	Vector3 GetWorldPosition() override{ return Vector3(); };
	// 角度を取得
	Vector3 GetRotation() override{ return Vector3(); };

};

