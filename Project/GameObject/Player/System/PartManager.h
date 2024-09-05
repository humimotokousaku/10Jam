#pragma once

class Player;

namespace PlayerContext
{
	class PartManager {
	public:
		// 初期化
		void Initialize(Player* player);
		// 更新
		void Update() {};
		// 追加
		void AddPart() {};

	private:
		Player* player_ = nullptr;

	};

}
