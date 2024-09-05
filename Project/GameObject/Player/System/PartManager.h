#pragma once
#include <memory>
#include <vector>
#include "Vector3.h"
//#include "../Player.h"

class IPart;
class Player;

namespace PlayerContext
{
	enum class DarumaPattern : uint32_t {
		kDefault,
	};

	class PartManager {
	public:
		// 初期化
		void Initialize(Player* player);
		// 更新
		void Update() {};
		// 追加
		void AddDaruma(DarumaPattern pattern);

		template<typename T>
		void AddParts(const Vector3& position);

	private:
		Player* player_ = nullptr;

	};

}
