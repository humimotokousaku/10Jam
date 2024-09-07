#pragma once
#include "Vector3.h"

namespace PlayerContext
{
	struct PhysicsParam {
		float mass;	// 質量
		float gravity; // 重力
		float frictionCoeff;	// 動摩擦係数
		float normalForce;	// 垂直抗力
	};

	class PhysicsSystem {
	public:
		static Vector3 ApplyX_ZFriction(const Vector3& velocity, PhysicsParam param);

	private:

	};
}

