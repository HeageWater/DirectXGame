#include "playerbullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position) {

	// NULLポインタチェック
	assert(model);

	//モデル
	this->model = model;

	//画像データ
	textureHandle = TextureManager::Load("black.png");

	//初期化
	worldTransform.Initialize();

	//場所代入
	worldTransform.translation_ = position;
}

void PlayerBullet::Update() {
	// worldTransform更新

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model->Draw(worldTransform, viewProjection, textureHandle);
}
