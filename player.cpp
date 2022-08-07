#include "player.h"

#define PI 3.1415;

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

	if (bullet) {
		bullet->Draw(viewProjection);
	}

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

	//y軸移動
	if (input->PushKey(DIK_W)) {
		move.y = speed;
	} else if (input->PushKey(DIK_S)) {
		move.y = -speed;
	}

	//x軸移動
	if (input->PushKey(DIK_D)) {
		move.x = speed;
	} else if (input->PushKey(DIK_A)) {
		move.x = -speed;
	}

	//回転
	if (input->PushKey(DIK_E)) {
		worldTransform.rotation_.z += 0.1f;
	}
	if (input->PushKey(DIK_Q)) {
		worldTransform.rotation_.z -= 0.1f;
	}

	//攻撃
	Attack();

	//弾更新
	if (bullet) {
		bullet->Update();
	}

	//移動限界
	const float kMoveLimitX = 32;
	const float kMoveLimitY = 18;

	worldTransform.translation_.x += move.x;
	worldTransform.translation_.y += move.y;
	worldTransform.translation_.z += move.z;

	//範囲を超えない処理
	worldTransform.translation_.x = max(worldTransform.translation_.x, -kMoveLimitX);
	worldTransform.translation_.x = min(worldTransform.translation_.x, +kMoveLimitX);
	worldTransform.translation_.y = max(worldTransform.translation_.y, -kMoveLimitY);
	worldTransform.translation_.y = min(worldTransform.translation_.y, +kMoveLimitY);

	//拡縮
	//Scale();

	//回転
	//Rota();

	//平行移動
	//Trans();

	UpdateMatrix(worldTransform);

	debugText->SetPos(50, 70);
	debugText->Printf("move:%f,%f,%f", move.x, move.y, move.z);
}

//攻撃
void Player::Attack() {
	if (input->PushKey(DIK_SPACE)) {
		//弾生成
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model, worldTransform.translation_);

		//弾登録
		bullet = newBullet;
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

//平行移動
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
	worldTransform.matWorld_.Reset();
	worldTransform.matWorld_ *= matRot;
	UpdateMatrix(worldTransform);
	//worldTransform.TransferMatrix();
}

//拡縮
void Player::Scale() {



}

//拡縮平行回転全部
void Player::UpdateMatrix(WorldTransform world) {

	Matrix4 matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = MathUtility::Matrix4Identity();
	matScale.m[0][0] = world.scale_.x;
	matScale.m[1][1] = world.scale_.y;
	matScale.m[2][2] = world.scale_.z;
	matScale.m[3][3] = 1;

	//回転
	matRot = MathUtility::Matrix4Identity();
	matRot = MathUtility::Matrix4RotationZ(world.rotation_.z);
	matRot *= MathUtility::Matrix4RotationX(world.rotation_.x);
	matRot *= MathUtility::Matrix4RotationY(world.rotation_.y);

	//移動
	matTrans = MathUtility::Matrix4Identity();
	matTrans = MathUtility::Matrix4Translation(
	  world.translation_.x, world.translation_.y, world.translation_.z);

	// ワールド行列の合成
	world.matWorld_ = MathUtility::Matrix4Identity(); // 変形をリセット
	world.matWorld_ *= matScale; // ワールド行列にスケーリングを反映
	world.matWorld_ *= matRot;   // ワールド行列に回転を反映
	world.matWorld_ *= matTrans; // ワールド行列に平行移動を反映

	world.TransferMatrix();
}