#pragma once

class Player;

namespace PlayerContext
{
	class DamageManager
	{
	public:
		void Initialize(Player* player) { player_ = player; }

	private:
		Player* player_ = nullptr;
	};

}