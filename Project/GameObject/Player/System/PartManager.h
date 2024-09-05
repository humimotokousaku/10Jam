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
		// ImGui
		void ImGuiDraw();

	public:
		// 追加
		template<typename T>
		void AddParts(const Vector3& position);
		void AddDaruma(DarumaPattern pattern);

	private:
		Player* player_ = nullptr;
		Vector3 generatePosition_;
	};

}
