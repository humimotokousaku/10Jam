#pragma once
#include "../IPart.h"

class LightPart : public IPart
{
public:
	~LightPart();
	void Initialize(CollisionManager* manager) override;
	void Update() override;
	void Draw() override;
	void ImGuiDraw() override;
	void ApplyGlobalVariables() override;
};

