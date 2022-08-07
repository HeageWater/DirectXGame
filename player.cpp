#include "player.h"

#define PI 3.1415;

//fBAÔ·
float ReturnRadian(float n) {
	n *= PI;
	n /= 180;
	return n;
}

//ú»
void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model = model;

	this->textureHandle = textureHandle;

	input = Input::GetInstance();
	debugText = DebugText::GetInstance();

	worldTransform.Initialize();
}

//`æ
void Player::Draw(ViewProjection viewProjection) {
	model->Draw(worldTransform, viewProjection, textureHandle);

	if (bullet) {
		bullet->Draw(viewProjection);
	}

	//ÀW\¦
	debugText->SetPos(50, 110);
	debugText->Printf(
	  "trans:%f,%f,%f", worldTransform.translation_.x, worldTransform.translation_.y,
	  worldTransform.translation_.z);
}

//XV
void Player::Update() {

	//Xs[h
	const float speed = 0.3f;

	//ÅIIÉransÉ«·l
	Vector3 move = {0, 0, 0};

	//y²Ú®
	if (input->PushKey(DIK_UP)) {
		move.y = speed;
	} else if (input->PushKey(DIK_DOWN)) {
		move.y = -speed;
	}

	//x²Ú®
	if (input->PushKey(DIK_RIGHT)) {
		move.x = speed;
	} else if (input->PushKey(DIK_LEFT)) {
		move.x = -speed;
	}

	//ñ]
	if (input->PushKey(DIK_E)) {
		worldTransform.rotation_.y += 0.1f;
	}
	if (input->PushKey(DIK_Q)) {
		worldTransform.rotation_.y -= 0.1f;
	}

	//U
	Attack();

	//eXV
	if (bullet) {
		bullet->Update();
	}

	//Ú®ÀE
	const float kMoveLimitX = 32;
	const float kMoveLimitY = 18;

	worldTransform.translation_.x += move.x;
	worldTransform.translation_.y += move.y;
	worldTransform.translation_.z += move.z;

	//ÍÍð´¦È¢
	worldTransform.translation_.x = max(worldTransform.translation_.x, -kMoveLimitX);
	worldTransform.translation_.x = min(worldTransform.translation_.x, +kMoveLimitX);
	worldTransform.translation_.y = max(worldTransform.translation_.y, -kMoveLimitY);
	worldTransform.translation_.y = min(worldTransform.translation_.y, +kMoveLimitY);

	//gk
	Scale();

	//ñ]
	Rota();

	//½sÚ®
	Trans();

	debugText->SetPos(50, 70);
	debugText->Printf("move:%f,%f,%f", move.x, move.y, move.z);
}

//U
void Player::Attack() {
	if (input->PushKey(DIK_SPACE)) {
		//e¶¬
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model, worldTransform.translation_);

		//eo^
		bullet = newBullet;
	}
}

//à¾
void moveee() {
	////Z²Ìñ]
	// worldTransform_.rotation_ = { 0.0f,0.0f,ReturnRadian(45.0f)};

	////Z²ñ]sñ
	// Matrix4 matrotZ;

	////PÊsñãü
	// matrotZ.Reset();

	// matrotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	// matrotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	// matrotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	// matrotZ.m[1][1] = cos(worldTransform_.rotation_.z);

	////PÊsñãü
	// worldTransform_.matWorld_.Reset();

	// worldTransform_.matWorld_ *= matrotZ;

	// worldTransform_.TransferMatrix();

	////X²Ìñ]
	// worldTransform_.rotation_ = {ReturnRadian(45.0f) , 0.0f,0.0f };

	// Matrix4 matrotX;

	////PÊsñãü
	// matrotX.Reset();

	// matrotX.m[1][1] = cos(worldTransform_.rotation_.x);
	// matrotX.m[1][2] = sin(worldTransform_.rotation_.x);
	// matrotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	// matrotX.m[2][2] = cos(worldTransform_.rotation_.x);

	// worldTransform_.matWorld_ *= matrotX;

	// worldTransform_.TransferMatrix();

	////Y²Ìñ]
	// worldTransform_.rotation_ = { 0.0f,ReturnRadian(45.0f) ,0.0f };

	// Matrix4 matrotY;

	////PÊsñãü
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

	////¬pñ]sñ
	// Matrix4 matRot;

	////e²Ìñ]sñ
	// Matrix4 matrotX, matrotY, matrotZ;

	////PÊsñãü
	// matrotZ.Reset();

	// matrotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	// matrotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	// matrotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	// matrotZ.m[1][1] = cos(worldTransform_.rotation_.z);

	////PÊsñãü
	// matrotX.Reset();

	// matrotX.m[1][1] = cos(worldTransform_.rotation_.x);
	// matrotX.m[1][2] = sin(worldTransform_.rotation_.x);
	// matrotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	// matrotX.m[2][2] = cos(worldTransform_.rotation_.x);

	////PÊsñãü
	// matrotY.Reset();

	// matrotY.m[0][0] = cos(worldTransform_.rotation_.y);
	// matrotY.m[0][2] = -sin(worldTransform_.rotation_.y);
	// matrotY.m[2][0] = sin(worldTransform_.rotation_.y);
	// matrotY.m[2][2] = cos(worldTransform_.rotation_.y);

	// matRot = matrotZ;
	// matRot *= matrotX;
	// matRot *= matrotY;

	////PÊsñãü
	////worldTransform_.matWorld_.Reset();
	// worldTransform_.matWorld_ *= matRot;
	// worldTransform_.TransferMatrix();

	////½sÚ®
	// worldTransform_.translation_ = { 5.0f,0.0f,0.0f };

	////½sÚ®sñé¾
	// Matrix4 matTrans = MathUtility::Matrix4Identity();

	// matTrans.m[3][0] = worldTransform_.translation_.x;
	// matTrans.m[3][1] = worldTransform_.translation_.y;
	// matTrans.m[3][2] = worldTransform_.translation_.z;

	////PÊsñãü
	////worldTransform_.matWorld_.Reset();
	// worldTransform_.matWorld_ *= matTrans;
	// worldTransform_.TransferMatrix();
}

//½sÚ®
void Player::Trans() {
	//½sÚ®sñé¾
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = worldTransform.translation_.x;
	matTrans.m[3][1] = worldTransform.translation_.y;
	matTrans.m[3][2] = worldTransform.translation_.z;

	//PÊsñãü
	worldTransform.matWorld_.Reset();
	worldTransform.matWorld_ *= matTrans;
	worldTransform.TransferMatrix();
}

//ñ]
void Player::Rota() {
	// Z²ñ]sñ
	Matrix4 matrotZ;

	//PÊsñãü
	matrotZ.Reset();

	// X²Ìñ]
	Matrix4 matrotX;

	//PÊsñãü
	matrotX.Reset();

	// Y²Ìñ]
	Matrix4 matrotY;

	//PÊsñãü
	matrotY.Reset();

	//¬pñ]sñ
	Matrix4 matRot;

	//PÊsñãü
	matrotZ.Reset();

	matrotZ.m[0][0] = cos(worldTransform.rotation_.z);
	matrotZ.m[0][1] = sin(worldTransform.rotation_.z);
	matrotZ.m[1][0] = -sin(worldTransform.rotation_.z);
	matrotZ.m[1][1] = cos(worldTransform.rotation_.z);

	//PÊsñãü
	matrotX.Reset();

	matrotX.m[1][1] = cos(worldTransform.rotation_.x);
	matrotX.m[1][2] = sin(worldTransform.rotation_.x);
	matrotX.m[2][1] = -sin(worldTransform.rotation_.x);
	matrotX.m[2][2] = cos(worldTransform.rotation_.x);

	//PÊsñãü
	matrotY.Reset();

	matrotY.m[0][0] = cos(worldTransform.rotation_.y);
	matrotY.m[0][2] = -sin(worldTransform.rotation_.y);
	matrotY.m[2][0] = sin(worldTransform.rotation_.y);
	matrotY.m[2][2] = cos(worldTransform.rotation_.y);

	matRot = matrotZ;
	matRot *= matrotX;
	matRot *= matrotY;

	//PÊsñãü
	worldTransform.matWorld_.Reset();
	worldTransform.matWorld_ *= matRot;
	worldTransform.TransferMatrix();
}

//gk
void Player::Scale() {



}