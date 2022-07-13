#include "player.h"
#include "Audio.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>

#define PI     3.1415;
#define XM_2PI 6.2835;

//ラディアン返す
float ReturnRadian(float n) {
	n *= PI;
	n /= 180;
	return n;
}

//初期化
void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model = model;

	this->textureHandle = textureHandle;

	input = Input::GetInstance();
	debugText = DebugText::GetInstance();

	worldTransform.Initialize();
}

//描画
void Player::Draw(ViewProjection viewProjection) {
	model->Draw(worldTransform, viewProjection, textureHandle);

	/*if (bullet) {
	    bullet->Draw(viewProjection);
	}*/

	//座標表示
	debugText->SetPos(50, 110);
	debugText->Printf(
	  "trans:%f,%f,%f", worldTransform.translation_.x, worldTransform.translation_.y,
	  worldTransform.translation_.z);
}

//更新
void Player::Update() {

	//スピード
	const float speed = 0.3f;

	//最終的にransに足す値
	Vector3 move = {0, 0, 0};

	// y軸移動
	if (input->PushKey(DIK_UP)) {
		move.y = speed;
	} else if (input->PushKey(DIK_DOWN)) {
		move.y = -speed;
	}

	// x軸移動
	if (input->PushKey(DIK_RIGHT)) {
		move.x = speed;
	} else if (input->PushKey(DIK_LEFT)) {
		move.x = -speed;
	}

	//回転
	if (input->PushKey(DIK_E)) {
		worldTransform.rotation_.y += 0.1f;
	}
	if (input->PushKey(DIK_Q)) {
		worldTransform.rotation_.y -= 0.1f;
	}

	if (input->PushKey(DIK_W)) {
		worldTransform.scale_.x += 0.1f;
	}

	//リセット
	if (input->PushKey(DIK_R)) {
		worldTransform.translation_ = {0, 0, 0};
		worldTransform.rotation_ = {0, 0, 0};
		worldTransform.scale_ = {1, 1, 1};
	}

	float rt = XM_2PI;

	worldTransform.rotation_.y = fmodf(worldTransform.rotation_.y, rt);

	//攻撃
	Attack();

	if (effectF) {
		Effect();
	}

	////弾更新
	// if (bullet) {
	//	bullet->Update();
	// }

	worldTransform.translation_.x += move.x;
	worldTransform.translation_.y += move.y;
	worldTransform.translation_.z += move.z;

	//移動限界
	const float kMoveLimitX = 32;
	const float kMoveLimitY = 18;

	//範囲を超えない処理
	worldTransform.translation_.x = max(worldTransform.translation_.x, -kMoveLimitX);
	worldTransform.translation_.x = min(worldTransform.translation_.x, +kMoveLimitX);
	worldTransform.translation_.y = max(worldTransform.translation_.y, -kMoveLimitY);
	worldTransform.translation_.y = min(worldTransform.translation_.y, +kMoveLimitY);

	//拡縮
	Scale();

	//平行移動
	Trans();

	//回転
	Rota();

	debugText->SetPos(50, 70);
	debugText->Printf("move:%f,%f,%f", worldTransform.scale_.x, move.y, move.z);
}

//攻撃
void Player::Attack() {
	if (input->PushKey(DIK_SPACE) && effectF != true) {
		//弾生成
		// PlayerBullet* newBullet = new PlayerBullet();
		// newBullet->Initialize(model, worldTransform.translation_);

		//弾登録
		// bullet = newBullet;

		effectF = true;

		worldTransform.scale_.x = 0;
		worldTransform.scale_.y = 0;
		worldTransform.scale_.z = 0;
	}
}

//説明
void moveee() {
	////Z軸の回転
	// worldTransform_.rotation_ = { 0.0f,0.0f,ReturnRadian(45.0f)};

	////Z軸回転行列
	// Matrix4 matrotZ;

	////単位行列代入
	// matrotZ.Reset();

	// matrotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	// matrotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	// matrotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	// matrotZ.m[1][1] = cos(worldTransform_.rotation_.z);

	////単位行列代入
	// worldTransform_.matWorld_.Reset();

	// worldTransform_.matWorld_ *= matrotZ;

	// worldTransform_.TransferMatrix();

	////X軸の回転
	// worldTransform_.rotation_ = {ReturnRadian(45.0f) , 0.0f,0.0f };

	// Matrix4 matrotX;

	////単位行列代入
	// matrotX.Reset();

	// matrotX.m[1][1] = cos(worldTransform_.rotation_.x);
	// matrotX.m[1][2] = sin(worldTransform_.rotation_.x);
	// matrotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	// matrotX.m[2][2] = cos(worldTransform_.rotation_.x);

	// worldTransform_.matWorld_ *= matrotX;

	// worldTransform_.TransferMatrix();

	////Y軸の回転
	// worldTransform_.rotation_ = { 0.0f,ReturnRadian(45.0f) ,0.0f };

	// Matrix4 matrotY;

	////単位行列代入
	// matrotY.Reset();

	// matrotY.m[0][0] = cos(worldTransform_.rotation_.y);
	// matrotY.m[0][2] = -sin(worldTransform_.rotation_.y);
	// matrotY.m[2][0] = sin(worldTransform_.rotation_.y);
	// matrotY.m[2][2] = cos(worldTransform_.rotation_.y);

	// worldTransform_.matWorld_ *= matrotY;

	// worldTransform_.TransferMatrix();

	////x,y,z
	// worldTransform_.rotation_ = { ReturnRadian(45.0f) ,ReturnRadian(45.0f) ,ReturnRadian(45.0f)
	// };

	////合成用回転行列
	// Matrix4 matRot;

	////各軸の回転行列
	// Matrix4 matrotX, matrotY, matrotZ;

	////単位行列代入
	// matrotZ.Reset();

	// matrotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	// matrotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	// matrotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	// matrotZ.m[1][1] = cos(worldTransform_.rotation_.z);

	////単位行列代入
	// matrotX.Reset();

	// matrotX.m[1][1] = cos(worldTransform_.rotation_.x);
	// matrotX.m[1][2] = sin(worldTransform_.rotation_.x);
	// matrotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	// matrotX.m[2][2] = cos(worldTransform_.rotation_.x);

	////単位行列代入
	// matrotY.Reset();

	// matrotY.m[0][0] = cos(worldTransform_.rotation_.y);
	// matrotY.m[0][2] = -sin(worldTransform_.rotation_.y);
	// matrotY.m[2][0] = sin(worldTransform_.rotation_.y);
	// matrotY.m[2][2] = cos(worldTransform_.rotation_.y);

	// matRot = matrotZ;
	// matRot *= matrotX;
	// matRot *= matrotY;

	////単位行列代入
	////worldTransform_.matWorld_.Reset();
	// worldTransform_.matWorld_ *= matRot;
	// worldTransform_.TransferMatrix();

	////平行移動
	// worldTransform_.translation_ = { 5.0f,0.0f,0.0f };

	////平行移動行列宣言
	// Matrix4 matTrans = MathUtility::Matrix4Identity();

	// matTrans.m[3][0] = worldTransform_.translation_.x;
	// matTrans.m[3][1] = worldTransform_.translation_.y;
	// matTrans.m[3][2] = worldTransform_.translation_.z;

	////単位行列代入
	////worldTransform_.matWorld_.Reset();
	// worldTransform_.matWorld_ *= matTrans;
	// worldTransform_.TransferMatrix();
}

//回転しながら大きく
void Player::Effect() {
	worldTransform.scale_.x = 2.0f;
	worldTransform.scale_.y = 2.0f;
	worldTransform.scale_.z = 2.0f;
}

//移動
void Player::Trans() {
	//平行移動行列宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = worldTransform.translation_.x;
	matTrans.m[3][1] = worldTransform.translation_.y;
	matTrans.m[3][2] = worldTransform.translation_.z;

	//単位行列代入
	worldTransform.matWorld_.Reset();
	worldTransform.matWorld_ *= matTrans;
	worldTransform.TransferMatrix();
}

//拡縮
void Player::Scale() {
	//スケ－リング行列
	Matrix4 matScale;
	matScale.m[0][0] = worldTransform.scale_.x;
	matScale.m[1][1] = worldTransform.scale_.y;
	matScale.m[2][2] = worldTransform.scale_.z;

	//単位行列代入
	worldTransform.matWorld_.Reset();
	worldTransform.matWorld_ *= matScale;
	worldTransform.TransferMatrix();
}

//回転
void Player::Rota() {
	// Z軸回転行列
	Matrix4 matrotZ;

	//単位行列代入
	matrotZ.Reset();

	// X軸の回転
	Matrix4 matrotX;

	//単位行列代入
	matrotX.Reset();

	// Y軸の回転
	Matrix4 matrotY;

	//単位行列代入
	matrotY.Reset();

	//合成用回転行列
	Matrix4 matRot;

	//単位行列代入
	matrotZ.Reset();

	matrotZ.m[0][0] = cos(worldTransform.rotation_.z);
	matrotZ.m[0][1] = sin(worldTransform.rotation_.z);
	matrotZ.m[1][0] = -sin(worldTransform.rotation_.z);
	matrotZ.m[1][1] = cos(worldTransform.rotation_.z);

	//単位行列代入
	matrotX.Reset();

	matrotX.m[1][1] = cos(worldTransform.rotation_.x);
	matrotX.m[1][2] = sin(worldTransform.rotation_.x);
	matrotX.m[2][1] = -sin(worldTransform.rotation_.x);
	matrotX.m[2][2] = cos(worldTransform.rotation_.x);

	//単位行列代入
	matrotY.Reset();

	matrotY.m[0][0] = cos(worldTransform.rotation_.y);
	matrotY.m[0][2] = -sin(worldTransform.rotation_.y);
	matrotY.m[2][0] = sin(worldTransform.rotation_.y);
	matrotY.m[2][2] = cos(worldTransform.rotation_.y);

	matRot = matrotZ;
	matRot *= matrotX;
	matRot *= matrotY;

	//単位行列代入
	// worldTransform.matWorld_.Reset();
	worldTransform.matWorld_ *= matRot;
	worldTransform.TransferMatrix();
}