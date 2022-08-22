#include "Enemy.h"

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = EnemyW.translation_.x;
	worldPos.y = EnemyW.translation_.y;
	worldPos.z = EnemyW.translation_.z;

	return worldPos;
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model = model;

	this->textureHandle = textureHandle;

	EnemyW.Initialize();

	EnemyW.translation_ = {0, 5, 10};
	EnemyW.scale_ = {2.0f, 2.0f, 2.0};
}

void Enemy::Draw(ViewProjection viewProjection) {
	model->Draw(EnemyW, viewProjection, textureHandle);

	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Update() {

	Ktimer--;

	if (Ktimer < 0) {
		Fire();
		Ktimer = Kfire;
	}

	//スピード
	const float speed = 0.3f;

	//最終的にransに足す値
	Vector3 move = {0, 0, -speed};

	switch (phase_) {

	case Phase::Stay:
		break;
	default:
		EnemyW.translation_ += move;
		break;
	}

	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Update();
	}

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
	EnemyW.matWorld_ *= matScale; // ワールド行列にスケーリングを反映
	EnemyW.matWorld_ *= matRot;   // ワールド行列に回転を反映
	EnemyW.matWorld_ *= matTrans; // ワールド行列に平行移動を反映

	EnemyW.TransferMatrix();
}

void Enemy::Fire() {
	assert(player_);

	//弾速
	const float kBulletSpeed = -0.01f;

	//Vector3 E = GetWorldPosition();
	//Vector3 P = GetWorldPosition();


	Vector3 velocity(0, 0, kBulletSpeed);


	//ベクトルと行列の掛け算
	velocity = velocity.mat(velocity, EnemyW.matWorld_);

	//弾生成
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model, EnemyW.translation_, velocity);

	//弾登録
	bullets.push_back(std::move(newBullet));
}

void Enemy::OnCollision() {

}