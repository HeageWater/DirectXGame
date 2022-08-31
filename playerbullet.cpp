#include "playerbullet.h"

Vector3 PlayerBullet::GetPosition() {
	Vector3 worldPos;

	worldPos.x = bulletW.translation_.x;
	worldPos.y = bulletW.translation_.y;
	worldPos.z = bulletW.translation_.z;

	return worldPos;
}

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity_) {

	// NULLポインタチェック
	assert(model);

	//モデル
	this->model = model;

	//画像データ
	textureHandle = TextureManager::Load("kyu//kyu.png");

	//初期化
	bulletW.Initialize();

	//場所代入
	bulletW.translation_ = position;

	bulletW.scale_ = {2.0f,2.0f,2.0f};

	//速度代入
	velocity = velocity_;
}

void PlayerBullet::Update() {

	bulletW.translation_ += velocity;

	if (--deathTimer <= 0) {

		isDead_ = true;
	}

	// worldTransform更新
	//スケ-リング行列の宣言
	Matrix4 matScale;

	//単位行列を代入
	matScale.Reset();
	//スケ-リング倍率を行列に設定する
	matScale.m[0][0] = bulletW.scale_.x;
	matScale.m[1][1] = bulletW.scale_.y;
	matScale.m[2][2] = bulletW.scale_.z;

	//単位行列を代入
	bulletW.matWorld_.Reset();

	// matScaleを掛けて代入
	bulletW.matWorld_.m[0][0] *= matScale.m[0][0];
	bulletW.matWorld_.m[1][1] *= matScale.m[1][1];
	bulletW.matWorld_.m[2][2] *= matScale.m[2][2];

	//合成用回転行列
	Matrix4 matRot;

	//各軸の回転行列
	Matrix4 matrotX, matrotY, matrotZ;

	//単位行列代入
	matrotZ.Reset();

	matrotZ.m[0][0] = cos(bulletW.rotation_.z);
	matrotZ.m[0][1] = sin(bulletW.rotation_.z);
	matrotZ.m[1][0] = -sin(bulletW.rotation_.z);
	matrotZ.m[1][1] = cos(bulletW.rotation_.z);

	//単位行列代入
	matrotX.Reset();

	matrotX.m[1][1] = cos(bulletW.rotation_.x);
	matrotX.m[1][2] = sin(bulletW.rotation_.x);
	matrotX.m[2][1] = -sin(bulletW.rotation_.x);
	matrotX.m[2][2] = cos(bulletW.rotation_.x);

	//単位行列代入
	matrotY.Reset();

	matrotY.m[0][0] = cos(bulletW.rotation_.y);
	matrotY.m[0][2] = -sin(bulletW.rotation_.y);
	matrotY.m[2][0] = sin(bulletW.rotation_.y);
	matrotY.m[2][2] = cos(bulletW.rotation_.y);

	matRot = matrotZ;
	matRot *= matrotX;
	matRot *= matrotY;

	//単位行列代入
	// worldTransform_.matWorld_.Reset();
	bulletW.matWorld_ *= matRot;
	bulletW.TransferMatrix();

	//平行移動行列宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = bulletW.translation_.x;
	matTrans.m[3][1] = bulletW.translation_.y;
	matTrans.m[3][2] = bulletW.translation_.z;

	//単位行列代入
	// worldTransform_.matWorld_.Reset();
	bulletW.matWorld_ *= matTrans;
	bulletW.TransferMatrix();
}

//弾描画
void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model->Draw(bulletW, viewProjection, textureHandle);
}

//当たり判定
void PlayerBullet::OnCollision() { isDead_ = true; }