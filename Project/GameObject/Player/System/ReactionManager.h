#pragma once
#include "Vector3.h"

class Player;

namespace PlayerContext
{
	struct PushParams {
		float power;
		float count;
	};

	class PartManager;

	class ReactionManager
	{
	public:
		// 初期化
		void Initialize(Player* player);
		// 押し出しアクション
		void PushAction(const Vector3& direct);
		void PushAction(const Vector3& direct, float power);

	private:
		Player* player_ = nullptr;
		PartManager* partManager_ = nullptr;
		// パラメータ
		PushParams params_;
	};


}
