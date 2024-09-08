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
		kHeavy,
		kL2M2H,
		//kM2L2H,
		kH2M2L,
	};

	class PartManager {
	public:
		// 初期化
		void Initialize(Player* player);
		// 更新
		void Update();
		// 描画
		void Draw();
		// ImGui
		void ImGuiDraw();
		void ImGuiPart();
	private:
		// 入力処理
		void InputUpdate();
	public:
		// 追加
		template<typename T>
		void AddParts(const Vector3& position);
		void AddHead(const Vector3& position);
		void AddDaruma(DarumaPattern pattern);

		std::vector<std::unique_ptr<IPart>>::iterator partsIt_;
		std::vector<std::unique_ptr<IPart>> parts_;

	private:
		Player* player_ = nullptr;
		Vector3 generatePosition_;

	};

}
