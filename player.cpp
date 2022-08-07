#include "player.h"

#define PI 3.1415;

//���f�B�A���Ԃ�
float ReturnRadian(float n) {
	n *= PI;
	n /= 180;
	return n;
}

//������
void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model = model;

	this->textureHandle = textureHandle;

	input = Input::GetInstance();
	debugText = DebugText::GetInstance();

	worldTransform.Initialize();
}

//�`��
void Player::Draw(ViewProjection viewProjection) {
	model->Draw(worldTransform, viewProjection, textureHandle);

	if (bullet) {
		bullet->Draw(viewProjection);
	}

	//���W�\��
	debugText->SetPos(50, 110);
	debugText->Printf(
	  "trans:%f,%f,%f", worldTransform.translation_.x, worldTransform.translation_.y,
	  worldTransform.translation_.z);
}

//�X�V
void Player::Update() {

	//�X�s�[�h
	const float speed = 0.3f;

	//�ŏI�I��rans�ɑ����l
	Vector3 move = {0, 0, 0};

	//y���ړ�
	if (input->PushKey(DIK_W)) {
		move.y = speed;
	} else if (input->PushKey(DIK_S)) {
		move.y = -speed;
	}

	//x���ړ�
	if (input->PushKey(DIK_D)) {
		move.x = speed;
	} else if (input->PushKey(DIK_A)) {
		move.x = -speed;
	}

	//��]
	if (input->PushKey(DIK_E)) {
		worldTransform.rotation_.z += 0.1f;
	}
	if (input->PushKey(DIK_Q)) {
		worldTransform.rotation_.z -= 0.1f;
	}

	//�U��
	Attack();

	//�e�X�V
	if (bullet) {
		bullet->Update();
	}

	//�ړ����E
	const float kMoveLimitX = 32;
	const float kMoveLimitY = 18;

	worldTransform.translation_.x += move.x;
	worldTransform.translation_.y += move.y;
	worldTransform.translation_.z += move.z;

	//�͈͂𒴂��Ȃ�����
	worldTransform.translation_.x = max(worldTransform.translation_.x, -kMoveLimitX);
	worldTransform.translation_.x = min(worldTransform.translation_.x, +kMoveLimitX);
	worldTransform.translation_.y = max(worldTransform.translation_.y, -kMoveLimitY);
	worldTransform.translation_.y = min(worldTransform.translation_.y, +kMoveLimitY);

	//�g�k
	//Scale();

	//��]
	//Rota();

	//���s�ړ�
	//Trans();

	UpdateMatrix(worldTransform);

	debugText->SetPos(50, 70);
	debugText->Printf("move:%f,%f,%f", move.x, move.y, move.z);
}

//�U��
void Player::Attack() {
	if (input->PushKey(DIK_SPACE)) {
		//�e����
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model, worldTransform.translation_);

		//�e�o�^
		bullet = newBullet;
	}
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

//���s�ړ�
void Player::Trans() {
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

//��]
void Player::Rota() {
	// Z����]�s��
	Matrix4 matrotZ;

	//�P�ʍs����
	matrotZ.Reset();

	// X���̉�]
	Matrix4 matrotX;

	//�P�ʍs����
	matrotX.Reset();

	// Y���̉�]
	Matrix4 matrotY;

	//�P�ʍs����
	matrotY.Reset();

	//�����p��]�s��
	Matrix4 matRot;

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
	UpdateMatrix(worldTransform);
	//worldTransform.TransferMatrix();
}

//�g�k
void Player::Scale() {



}

//�g�k���s��]�S��
void Player::UpdateMatrix(WorldTransform world) {

	Matrix4 matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = MathUtility::Matrix4Identity();
	matScale.m[0][0] = world.scale_.x;
	matScale.m[1][1] = world.scale_.y;
	matScale.m[2][2] = world.scale_.z;
	matScale.m[3][3] = 1;

	//��]
	matRot = MathUtility::Matrix4Identity();
	matRot = MathUtility::Matrix4RotationZ(world.rotation_.z);
	matRot *= MathUtility::Matrix4RotationX(world.rotation_.x);
	matRot *= MathUtility::Matrix4RotationY(world.rotation_.y);

	//�ړ�
	matTrans = MathUtility::Matrix4Identity();
	matTrans = MathUtility::Matrix4Translation(
	  world.translation_.x, world.translation_.y, world.translation_.z);

	// ���[���h�s��̍���
	world.matWorld_ = MathUtility::Matrix4Identity(); // �ό`�����Z�b�g
	world.matWorld_ *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	world.matWorld_ *= matRot;   // ���[���h�s��ɉ�]�𔽉f
	world.matWorld_ *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	world.TransferMatrix();
}