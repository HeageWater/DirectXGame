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

	//回転フラグ
	bool rotaFlg = false;

	//
	bool transFlg = false;

	//y軸移動
	if (input->PushKey(DIK_UP)) {
		move.y = speed;
		transFlg = true;
	} else if (input->PushKey(DIK_DOWN)) {
		move.y = -speed;
		transFlg = true;
	}

	//x軸移動
	if (input->PushKey(DIK_RIGHT)) {
		move.x = speed;
		transFlg = true;
	} else if (input->PushKey(DIK_LEFT)) {
		move.x = -speed;
		transFlg = true;
	}

	//回転
	if (input->PushKey(DIK_E)) {
		worldTransform.rotation_.y += 0.1f;
		rotaFlg = true;
	}
	if (input->PushKey(DIK_Q)) {
		worldTransform.rotation_.y -= 0.1f;
		rotaFlg = true;
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

	//範囲を超えない処理
	worldTransform.translation_.x = max(worldTransform.translation_.x, -kMoveLimitX);
	worldTransform.translation_.x = min(worldTransform.translation_.x, +kMoveLimitX);
	worldTransform.translation_.y = max(worldTransform.translation_.y, -kMoveLimitY);
	worldTransform.translation_.y = min(worldTransform.translation_.y, +kMoveLimitY);

	worldTransform.translation_.x += move.x;
	worldTransform.translation_.y += move.y;
	worldTransform.translation_.z += move.z;

	//平行移動
	if (transFlg) {
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
	if (rotaFlg) {
		// Z軸回転行列
		Matrix4 matrotZ;

		//単位行列代入
		matrotZ.Reset();

		matrotZ.m[0][0] = cos(worldTransform.rotation_.z);
		matrotZ.m[0][1] = sin(worldTransform.rotation_.z);
		matrotZ.m[1][0] = -sin(worldTransform.rotation_.z);
		matrotZ.m[1][1] = cos(worldTransform.rotation_.z);

		//単位行列代入
		worldTransform.matWorld_.Reset();

		worldTransform.matWorld_ *= matrotZ;

		worldTransform.TransferMatrix();

		// X軸の回転
		Matrix4 matrotX;

		//単位行列代入
		matrotX.Reset();

		matrotX.m[1][1] = cos(worldTransform.rotation_.x);
		matrotX.m[1][2] = sin(worldTransform.rotation_.x);
		matrotX.m[2][1] = -sin(worldTransform.rotation_.x);
		matrotX.m[2][2] = cos(worldTransform.rotation_.x);

		worldTransform.matWorld_ *= matrotX;

		worldTransform.TransferMatrix();

		// Y軸の回転
		Matrix4 matrotY;

		//単位行列代入
		matrotY.Reset();

		matrotY.m[0][0] = cos(worldTransform.rotation_.y);
		matrotY.m[0][2] = -sin(worldTransform.rotation_.y);
		matrotY.m[2][0] = sin(worldTransform.rotation_.y);
		matrotY.m[2][2] = cos(worldTransform.rotation_.y);

		worldTransform.matWorld_ *= matrotY;

		worldTransform.TransferMatrix();

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
		worldTransform.TransferMatrix();
	}

	debugText->SetPos(50, 70);
	debugText->Printf("move:%f,%f,%f", move.x, move.y, move.z);
}

//攻撃
void Player::Attack() {

	//弾生成
	PlayerBullet* newBullet = new PlayerBullet();
	newBullet->Initialize(model, worldTransform.translation_);

	//弾登録
	bullet = newBullet;

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