#include "Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model = model;

	this->textureHandle = textureHandle;

	EnemyW.Initialize();

	EnemyW.translation_ = {0,5,50};
}

void Enemy::Draw(ViewProjection viewProjection) {
	model->Draw(EnemyW, viewProjection, textureHandle);
}

void Enemy::Update() {
	//スピード
	const float speed = 0.3f;

	//最終的にransに足す値
	Vector3 move = {0, 0, -speed};

	EnemyW.translation_ += move;

	//上全部
	UpdateMatrix();
}

//拡縮平行回転全部
void Enemy::UpdateMatrix() {

	Matrix4 matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = MathUtility::Matrix4Identity();
	matScale.m[0][0] = EnemyW.scale_.x;
	matScale.m[1][1] = EnemyW.scale_.y;
	matScale.m[2][2] = EnemyW.scale_.z;
	matScale.m[3][3] = 1;

	//回転
	matRot = MathUtility::Matrix4Identity();
	matRot = MathUtility::Matrix4RotationZ(EnemyW.rotation_.z);
	matRot *= MathUtility::Matrix4RotationX(EnemyW.rotation_.x);
	matRot *= MathUtility::Matrix4RotationY(EnemyW.rotation_.y);

	//移動
	matTrans = MathUtility::Matrix4Identity();
	matTrans = MathUtility::Matrix4Translation(
	  EnemyW.translation_.x, EnemyW.translation_.y, EnemyW.translation_.z);

	// ワールド行列の合成
	EnemyW.matWorld_ = MathUtility::Matrix4Identity(); // 変形をリセット
	EnemyW.matWorld_ *= matScale;  // ワールド行列にスケーリングを反映
	EnemyW.matWorld_ *= matRot;   // ワールド行列に回転を反映
	EnemyW.matWorld_ *= matTrans;      // ワールド行列に平行移動を反映

	EnemyW.TransferMatrix();
}
