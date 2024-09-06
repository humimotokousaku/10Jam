#pragma once
#include "../IPart.h"

class MediumPart : public IPart
{
public:
	void Initialize(CollisionManager* manager) override;
	void Update() override;
	void Draw() override;
	void ImGuiDraw() override;

};
