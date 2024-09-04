#pragma once
#include <memory>

class IPart;
class Camera;
class CollisionManager;

class Player
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public: // アクセッサ
	void SetCollisionManager(CollisionManager* collisionManager) { collisionManager_ = collisionManager; }

private:
	// 試しのパーツ（土台
	std::unique_ptr<IPart> testPart_;
	// ポインタ
	Camera* camera_ = nullptr;
	CollisionManager* collisionManager_ = nullptr;
};
