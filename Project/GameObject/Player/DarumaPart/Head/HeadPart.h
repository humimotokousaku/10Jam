#pragma once
#include "../IPart.h"

class HeadPart : public IPart
{
public:
	void Initialize(CollisionManager* manager) override;
	void Update() override;
	void Draw() override;
	void ImGuiDraw() override;
	void AddTorque(Collider* collider);
	void OnCollision(Collider* collider) override;
private:
	uint32_t texture_;
};
