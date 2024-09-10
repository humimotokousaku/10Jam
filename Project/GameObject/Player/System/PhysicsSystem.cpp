#include "PhysicsSystem.h"
#include "GameObjectLists.h"

Vector3 PlayerContext::PhysicsSystem::ApplyX_ZFriction(const Vector3& velocity, PhysicsParam param)
{
	Vector3 acceleration = {};
	// 正規化
	Vector3 normalVelocity = Normalize(velocity);
	// 力
	Vector3 frictionForce{};
	if (Length(velocity) > 0.01f) {
		frictionForce = (normalVelocity * -param.frictionCoeff) * param.normalForce;
	}
	// 加速度を計算
	Vector3 totalForce = frictionForce;
	acceleration = totalForce / param.mass;
	// 制限
	if (std::fabsf(acceleration.x * (1.0f / 60.0f)) > std::fabsf(velocity.x)) {
		acceleration.x = velocity.x * 60.0f;
	}
	if (std::fabsf(acceleration.z * (1.0f / 60.0f)) > std::fabsf(velocity.z)) {
		acceleration.z = velocity.z * 60.0f;
	}
	// 速度を更新（加速度を考慮）
	Vector3 newVelocity = velocity;
	newVelocity.x += (acceleration.x * (1.0f / 60.0f));
	newVelocity.z += (acceleration.z * (1.0f / 60.0f));

	return Vector3(newVelocity);
}
