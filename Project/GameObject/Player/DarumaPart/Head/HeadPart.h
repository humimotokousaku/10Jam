#pragma once
#include "../IPart.h"

class HeadPart : public IPart
{
public:
	void Initialize(CollisionManager* manager) override;
	void Update() override;
	void Draw() override;
	void ImGuiDraw() override;
private:
	uint32_t texture_;
};
