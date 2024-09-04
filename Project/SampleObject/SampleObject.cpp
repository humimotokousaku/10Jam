#include "SampleObject.h"

SampleObject::~SampleObject() {
	models_.clear();
	// コライダリストに登録したものを削除
	collisionManager_->ClearColliderList(this);
}

void SampleObject::Init() {
	// 自機モデル作成(初期化より後にモデルのセットを行うこと)
	object3d_ = std::make_unique<Object3D>();
	object3d_->Initialize();
	object3d_->SetModel(models_[0]);
	object3d_->SetCamera(camera_);

	// colliderの設定
	SetCollisionPrimitive(kCollisionOBB);
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(~kCollisionAttributePlayer);
	collisionManager_->SetColliderList(this);
}

void SampleObject::Update() {

}

void SampleObject::Draw() {
	// テクスチャの番号を指定しない場合はモデルに使用しているテクスチャを使用する
	object3d_->Draw();
	// 読みこんだテクスチャを使用する場合
	//object3d_->Draw(textureHandle);
}

Vector3 SampleObject::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos{};
	// ワールド行列の平行移動成分を取得
	worldPos.x = object3d_->worldTransform.matWorld_.m[3][0];
	worldPos.y = object3d_->worldTransform.matWorld_.m[3][1];
	worldPos.z = object3d_->worldTransform.matWorld_.m[3][2];

	return worldPos;
}

Vector3 SampleObject::GetRotation() {
	Vector3 rotate = object3d_->worldTransform.rotate;
	return rotate;
}

void SampleObject::OnCollision(Collider* collider) {
	
}
