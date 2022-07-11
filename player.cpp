#include "player.h"

float ReturnRadian(float a) {

}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model = model;

	this->textureHandle = textureHandle;

	input = Input::GetInstance();
	debugText = DebugText::GetInstance();

	worldTransform.Initialize();
}

void Player::Draw(ViewProjection viewProjection) {
	model->Draw(worldTransform, viewProjection, textureHandle);

	debugText->SetPos(50, 110);
	debugText->Printf(
	  "trans:%f,%f,%f", worldTransform.translation_.x, worldTransform.translation_.y,
	  worldTransform.translation_.z);
}

void Player::Update() {
	const float speed = 0.3f;

	Vector3 move = {0, 0, 0};

	if (input->PushKey(DIK_UP)) {
		move.y = speed;
	} else if (input->PushKey(DIK_DOWN)) {
		move.y = -speed;
	}

	if (input->PushKey(DIK_RIGHT)) {
		move.x = speed;
	} else if (input->PushKey(DIK_LEFT)) {
		move.x = -speed;
	}

	if (input->PushKey(DIK_SPACE)) {
		move.z = speed;
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

	if (move.x != 0 || move.y != 0 || move.z != 0) {
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

	if (true) {
		// Z軸の回転
		worldTransform.rotation_ = {0.0f, 0.0f, ReturnRadian(45.0f)};

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
		worldTransform.rotation_ = {ReturnRadian(45.0f), 0.0f, 0.0f};

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
		worldTransform.rotation_ = {0.0f, ReturnRadian(45.0f), 0.0f};

		Matrix4 matrotY;

		//単位行列代入
		matrotY.Reset();

		matrotY.m[0][0] = cos(worldTransform.rotation_.y);
		matrotY.m[0][2] = -sin(worldTransform.rotation_.y);
		matrotY.m[2][0] = sin(worldTransform.rotation_.y);
		matrotY.m[2][2] = cos(worldTransform.rotation_.y);

		worldTransform.matWorld_ *= matrotY;

		worldTransform.TransferMatrix();

		// x,y,z
		worldTransform.rotation_ = {ReturnRadian(45.0f), ReturnRadian(45.0f), ReturnRadian(45.0f)};

		//合成用回転行列
		Matrix4 matRot;

		//各軸の回転行列
		Matrix4 matrotX, matrotY, matrotZ;

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