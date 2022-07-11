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

	//�ړ����E
	const float kMoveLimitX = 32;
	const float kMoveLimitY = 18;

	//�͈͂𒴂��Ȃ�����
	worldTransform.translation_.x = max(worldTransform.translation_.x, -kMoveLimitX);
	worldTransform.translation_.x = min(worldTransform.translation_.x, +kMoveLimitX);
	worldTransform.translation_.y = max(worldTransform.translation_.y, -kMoveLimitY);
	worldTransform.translation_.y = min(worldTransform.translation_.y, +kMoveLimitY);

	worldTransform.translation_.x += move.x;
	worldTransform.translation_.y += move.y;
	worldTransform.translation_.z += move.z;

	if (move.x != 0 || move.y != 0 || move.z != 0) {
		//���s�ړ��s��錾
		Matrix4 matTrans = MathUtility::Matrix4Identity();

		matTrans.m[3][0] = worldTransform.translation_.x;
		matTrans.m[3][1] = worldTransform.translation_.y;
		matTrans.m[3][2] = worldTransform.translation_.z;

		//�P�ʍs����
		worldTransform.matWorld_.Reset();
		worldTransform.matWorld_ *= matTrans;
		worldTransform.TransferMatrix();
	}

	if (true) {
		// Z���̉�]
		worldTransform.rotation_ = {0.0f, 0.0f, ReturnRadian(45.0f)};

		// Z����]�s��
		Matrix4 matrotZ;

		//�P�ʍs����
		matrotZ.Reset();

		matrotZ.m[0][0] = cos(worldTransform.rotation_.z);
		matrotZ.m[0][1] = sin(worldTransform.rotation_.z);
		matrotZ.m[1][0] = -sin(worldTransform.rotation_.z);
		matrotZ.m[1][1] = cos(worldTransform.rotation_.z);

		//�P�ʍs����
		worldTransform.matWorld_.Reset();

		worldTransform.matWorld_ *= matrotZ;

		worldTransform.TransferMatrix();

		// X���̉�]
		worldTransform.rotation_ = {ReturnRadian(45.0f), 0.0f, 0.0f};

		Matrix4 matrotX;

		//�P�ʍs����
		matrotX.Reset();

		matrotX.m[1][1] = cos(worldTransform.rotation_.x);
		matrotX.m[1][2] = sin(worldTransform.rotation_.x);
		matrotX.m[2][1] = -sin(worldTransform.rotation_.x);
		matrotX.m[2][2] = cos(worldTransform.rotation_.x);

		worldTransform.matWorld_ *= matrotX;

		worldTransform.TransferMatrix();

		// Y���̉�]
		worldTransform.rotation_ = {0.0f, ReturnRadian(45.0f), 0.0f};

		Matrix4 matrotY;

		//�P�ʍs����
		matrotY.Reset();

		matrotY.m[0][0] = cos(worldTransform.rotation_.y);
		matrotY.m[0][2] = -sin(worldTransform.rotation_.y);
		matrotY.m[2][0] = sin(worldTransform.rotation_.y);
		matrotY.m[2][2] = cos(worldTransform.rotation_.y);

		worldTransform.matWorld_ *= matrotY;

		worldTransform.TransferMatrix();

		// x,y,z
		worldTransform.rotation_ = {ReturnRadian(45.0f), ReturnRadian(45.0f), ReturnRadian(45.0f)};

		//�����p��]�s��
		Matrix4 matRot;

		//�e���̉�]�s��
		Matrix4 matrotX, matrotY, matrotZ;

		//�P�ʍs����
		matrotZ.Reset();

		matrotZ.m[0][0] = cos(worldTransform.rotation_.z);
		matrotZ.m[0][1] = sin(worldTransform.rotation_.z);
		matrotZ.m[1][0] = -sin(worldTransform.rotation_.z);
		matrotZ.m[1][1] = cos(worldTransform.rotation_.z);

		//�P�ʍs����
		matrotX.Reset();

		matrotX.m[1][1] = cos(worldTransform.rotation_.x);
		matrotX.m[1][2] = sin(worldTransform.rotation_.x);
		matrotX.m[2][1] = -sin(worldTransform.rotation_.x);
		matrotX.m[2][2] = cos(worldTransform.rotation_.x);

		//�P�ʍs����
		matrotY.Reset();

		matrotY.m[0][0] = cos(worldTransform.rotation_.y);
		matrotY.m[0][2] = -sin(worldTransform.rotation_.y);
		matrotY.m[2][0] = sin(worldTransform.rotation_.y);
		matrotY.m[2][2] = cos(worldTransform.rotation_.y);

		matRot = matrotZ;
		matRot *= matrotX;
		matRot *= matrotY;

		//�P�ʍs����
		worldTransform.matWorld_.Reset();
		worldTransform.matWorld_ *= matRot;
		worldTransform.TransferMatrix();
	}

	debugText->SetPos(50, 70);
	debugText->Printf("move:%f,%f,%f", move.x, move.y, move.z);
}

//����
void moveee() {
	////Z���̉�]
	// worldTransform_.rotation_ = { 0.0f,0.0f,ReturnRadian(45.0f)};

	////Z����]�s��
	// Matrix4 matrotZ;

	////�P�ʍs����
	// matrotZ.Reset();

	// matrotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	// matrotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	// matrotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	// matrotZ.m[1][1] = cos(worldTransform_.rotation_.z);

	////�P�ʍs����
	// worldTransform_.matWorld_.Reset();

	// worldTransform_.matWorld_ *= matrotZ;

	// worldTransform_.TransferMatrix();

	////X���̉�]
	// worldTransform_.rotation_ = {ReturnRadian(45.0f) , 0.0f,0.0f };

	// Matrix4 matrotX;

	////�P�ʍs����
	// matrotX.Reset();

	// matrotX.m[1][1] = cos(worldTransform_.rotation_.x);
	// matrotX.m[1][2] = sin(worldTransform_.rotation_.x);
	// matrotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	// matrotX.m[2][2] = cos(worldTransform_.rotation_.x);

	// worldTransform_.matWorld_ *= matrotX;

	// worldTransform_.TransferMatrix();

	////Y���̉�]
	// worldTransform_.rotation_ = { 0.0f,ReturnRadian(45.0f) ,0.0f };

	// Matrix4 matrotY;

	////�P�ʍs����
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

	////�����p��]�s��
	// Matrix4 matRot;

	////�e���̉�]�s��
	// Matrix4 matrotX, matrotY, matrotZ;

	////�P�ʍs����
	// matrotZ.Reset();

	// matrotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	// matrotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	// matrotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	// matrotZ.m[1][1] = cos(worldTransform_.rotation_.z);

	////�P�ʍs����
	// matrotX.Reset();

	// matrotX.m[1][1] = cos(worldTransform_.rotation_.x);
	// matrotX.m[1][2] = sin(worldTransform_.rotation_.x);
	// matrotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	// matrotX.m[2][2] = cos(worldTransform_.rotation_.x);

	////�P�ʍs����
	// matrotY.Reset();

	// matrotY.m[0][0] = cos(worldTransform_.rotation_.y);
	// matrotY.m[0][2] = -sin(worldTransform_.rotation_.y);
	// matrotY.m[2][0] = sin(worldTransform_.rotation_.y);
	// matrotY.m[2][2] = cos(worldTransform_.rotation_.y);

	// matRot = matrotZ;
	// matRot *= matrotX;
	// matRot *= matrotY;

	////�P�ʍs����
	////worldTransform_.matWorld_.Reset();
	// worldTransform_.matWorld_ *= matRot;
	// worldTransform_.TransferMatrix();

	////���s�ړ�
	// worldTransform_.translation_ = { 5.0f,0.0f,0.0f };

	////���s�ړ��s��錾
	// Matrix4 matTrans = MathUtility::Matrix4Identity();

	// matTrans.m[3][0] = worldTransform_.translation_.x;
	// matTrans.m[3][1] = worldTransform_.translation_.y;
	// matTrans.m[3][2] = worldTransform_.translation_.z;

	////�P�ʍs����
	////worldTransform_.matWorld_.Reset();
	// worldTransform_.matWorld_ *= matTrans;
	// worldTransform_.TransferMatrix();
}