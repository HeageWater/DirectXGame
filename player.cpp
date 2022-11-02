#include "player.h"

#define PI 3.1415;

void Player::Reset() {

	playerWorldTransform.translation_ = {0, 0, -20};
	playerWorldTransform.scale_ = {3.0f, 3.0f, 3.0f};

	isDead_ = false;
}

//ラディアン返す
float ReturnRadian(float n) {
	n *= PI;
	n /= 180;
	return n;
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = playerWorldTransform.translation_.x;
	worldPos.y = playerWorldTransform.translation_.y;
	worldPos.z = playerWorldTransform.translation_.z;

	return worldPos;
}

//初期化
void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model = model;

	this->textureHandle = textureHandle;

	input = Input::GetInstance();
	debugText = DebugText::GetInstance();

	playerWorldTransform.Initialize();

	playerWorldTransform.translation_ = {0, 0, -20};
	playerWorldTransform.scale_ = {3.0f, 3.0f, 3.0f};
}

//描画
void Player::Draw(ViewProjection viewProjection) {
	model->Draw(playerWorldTransform, viewProjection, textureHandle);
}

//更新
void Player::Update(ViewProjection viewProjection) {

	//スピード
	const float speed = 0.3f;

	//最終的にransに足す値
	Vector3 move = {0, 0, 0};

	// x軸移動
	if (input->PushKey(DIK_RIGHT)) {
		move.x = speed;
	} else if (input->PushKey(DIK_LEFT)) {
		move.x = -speed;
	}

	// z軸移動
	if (input->PushKey(DIK_UP)) {
		move.z = speed;
	} else if (input->PushKey(DIK_DOWN)) {
		move.z = -speed;
	}

	//ジャンプ
	if (Gravity < MaxGravity) {
		Gravity += 0.02f;
	}

	Jump();

	if (jump > 0) {
		playerWorldTransform.translation_.y += jump;

		jump -= 0.1f;
	} else {
		playerWorldTransform.translation_.y -= Gravity;
	}

	//移動限界
	const float kMoveLimitX = 48;
	const float kMoveLimitZ = 48;
	const float kMoveLimitY = 14;

	//移動
	move = move.mat(move, playerWorldTransform.matWorld_);

	playerWorldTransform.translation_.x += move.x;
	playerWorldTransform.translation_.y += move.y;
	playerWorldTransform.translation_.z += move.z;

	//範囲を超えない処理
	playerWorldTransform.translation_.x = max(playerWorldTransform.translation_.x, -kMoveLimitX);
	playerWorldTransform.translation_.x = min(playerWorldTransform.translation_.x, +kMoveLimitX);
	playerWorldTransform.translation_.y = max(playerWorldTransform.translation_.y, -kMoveLimitY);
	playerWorldTransform.translation_.y = min(playerWorldTransform.translation_.y, +kMoveLimitY);
	playerWorldTransform.translation_.z = max(playerWorldTransform.translation_.z, -kMoveLimitZ);
	playerWorldTransform.translation_.z = min(playerWorldTransform.translation_.z, +kMoveLimitZ);

	//上全部
	UpdateMatrix();
}

//平行移動
void Player::Trans() {
	//平行移動行列宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = playerWorldTransform.translation_.x;
	matTrans.m[3][1] = playerWorldTransform.translation_.y;
	matTrans.m[3][2] = playerWorldTransform.translation_.z;

	//単位行列代入
	playerWorldTransform.matWorld_.Reset();
	playerWorldTransform.matWorld_ *= matTrans;
	playerWorldTransform.TransferMatrix();
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

	matrotZ.m[0][0] = cos(playerWorldTransform.rotation_.z);
	matrotZ.m[0][1] = sin(playerWorldTransform.rotation_.z);
	matrotZ.m[1][0] = -sin(playerWorldTransform.rotation_.z);
	matrotZ.m[1][1] = cos(playerWorldTransform.rotation_.z);

	//単位行列代入
	matrotX.Reset();

	matrotX.m[1][1] = cos(playerWorldTransform.rotation_.x);
	matrotX.m[1][2] = sin(playerWorldTransform.rotation_.x);
	matrotX.m[2][1] = -sin(playerWorldTransform.rotation_.x);
	matrotX.m[2][2] = cos(playerWorldTransform.rotation_.x);

	//単位行列代入
	matrotY.Reset();

	matrotY.m[0][0] = cos(playerWorldTransform.rotation_.y);
	matrotY.m[0][2] = -sin(playerWorldTransform.rotation_.y);
	matrotY.m[2][0] = sin(playerWorldTransform.rotation_.y);
	matrotY.m[2][2] = cos(playerWorldTransform.rotation_.y);

	matRot = matrotZ;
	matRot *= matrotX;
	matRot *= matrotY;

	//単位行列代入
	playerWorldTransform.matWorld_.Reset();
	playerWorldTransform.matWorld_ *= matRot;
	UpdateMatrix();
	// worldTransform.TransferMatrix();
}

//拡縮
void Player::Scale() {}

//拡縮平行回転全部
void Player::UpdateMatrix() {

	Matrix4 matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = MathUtility::Matrix4Identity();
	matScale.m[0][0] = playerWorldTransform.scale_.x;
	matScale.m[1][1] = playerWorldTransform.scale_.y;
	matScale.m[2][2] = playerWorldTransform.scale_.z;
	matScale.m[3][3] = 1;

	//回転
	matRot = MathUtility::Matrix4Identity();
	matRot = MathUtility::Matrix4RotationZ(playerWorldTransform.rotation_.z);
	matRot *= MathUtility::Matrix4RotationX(playerWorldTransform.rotation_.x);
	matRot *= MathUtility::Matrix4RotationY(playerWorldTransform.rotation_.y);

	//移動
	matTrans = MathUtility::Matrix4Identity();
	matTrans = MathUtility::Matrix4Translation(
	  playerWorldTransform.translation_.x, playerWorldTransform.translation_.y,
	  playerWorldTransform.translation_.z);

	// ワールド行列の合成
	playerWorldTransform.matWorld_ = MathUtility::Matrix4Identity(); // 変形をリセット
	playerWorldTransform.matWorld_ *= matScale; // ワールド行列にスケーリングを反映
	playerWorldTransform.matWorld_ *= matRot;   // ワールド行列に回転を反映
	playerWorldTransform.matWorld_ *= matTrans; // ワールド行列に平行移動を反映

	playerWorldTransform.TransferMatrix();
}

//拡縮平行回転全部
void Player::UpdateMatrix(WorldTransform W) {

	Matrix4 matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = MathUtility::Matrix4Identity();
	matScale.m[0][0] = W.scale_.x;
	matScale.m[1][1] = W.scale_.y;
	matScale.m[2][2] = W.scale_.z;
	matScale.m[3][3] = 1;

	//回転
	matRot = MathUtility::Matrix4Identity();
	matRot = MathUtility::Matrix4RotationZ(W.rotation_.z);
	matRot *= MathUtility::Matrix4RotationX(W.rotation_.x);
	matRot *= MathUtility::Matrix4RotationY(W.rotation_.y);

	//移動
	matTrans = MathUtility::Matrix4Identity();
	matTrans =
	  MathUtility::Matrix4Translation(W.translation_.x, W.translation_.y, W.translation_.z);

	// ワールド行列の合成
	W.matWorld_ = MathUtility::Matrix4Identity(); // 変形をリセット
	W.matWorld_ *= matScale; // ワールド行列にスケーリングを反映
	W.matWorld_ *= matRot;   // ワールド行列に回転を反映
	W.matWorld_ *= matTrans; // ワールド行列に平行移動を反映

	W.TransferMatrix();
}

//ジャンプ
void Player::Jump() {
	if (input->TriggerKey(DIK_SPACE)) {

		if (playerWorldTransform.translation_.y < -13.9) {

			Gravity = 0;

			jump = Maxjump;
		}
	}
}
