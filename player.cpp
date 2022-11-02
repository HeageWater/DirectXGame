#include "player.h"

#define PI 3.1415;

void Player::Reset() {

	playerWorldTransform.translation_ = {0, 0, -20};
	playerWorldTransform.scale_ = {3.0f, 3.0f, 3.0f};

	isDead_ = false;
}

//���f�B�A���Ԃ�
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

//������
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

//�`��
void Player::Draw(ViewProjection viewProjection) {
	model->Draw(playerWorldTransform, viewProjection, textureHandle);
}

//�X�V
void Player::Update(ViewProjection viewProjection) {

	//�X�s�[�h
	const float speed = 0.3f;

	//�ŏI�I��rans�ɑ����l
	Vector3 move = {0, 0, 0};

	// x���ړ�
	if (input->PushKey(DIK_RIGHT)) {
		move.x = speed;
	} else if (input->PushKey(DIK_LEFT)) {
		move.x = -speed;
	}

	// z���ړ�
	if (input->PushKey(DIK_UP)) {
		move.z = speed;
	} else if (input->PushKey(DIK_DOWN)) {
		move.z = -speed;
	}

	//�W�����v
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

	//�ړ����E
	const float kMoveLimitX = 48;
	const float kMoveLimitZ = 48;
	const float kMoveLimitY = 14;

	//�ړ�
	move = move.mat(move, playerWorldTransform.matWorld_);

	playerWorldTransform.translation_.x += move.x;
	playerWorldTransform.translation_.y += move.y;
	playerWorldTransform.translation_.z += move.z;

	//�͈͂𒴂��Ȃ�����
	playerWorldTransform.translation_.x = max(playerWorldTransform.translation_.x, -kMoveLimitX);
	playerWorldTransform.translation_.x = min(playerWorldTransform.translation_.x, +kMoveLimitX);
	playerWorldTransform.translation_.y = max(playerWorldTransform.translation_.y, -kMoveLimitY);
	playerWorldTransform.translation_.y = min(playerWorldTransform.translation_.y, +kMoveLimitY);
	playerWorldTransform.translation_.z = max(playerWorldTransform.translation_.z, -kMoveLimitZ);
	playerWorldTransform.translation_.z = min(playerWorldTransform.translation_.z, +kMoveLimitZ);

	//��S��
	UpdateMatrix();
}

//���s�ړ�
void Player::Trans() {
	//���s�ړ��s��錾
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = playerWorldTransform.translation_.x;
	matTrans.m[3][1] = playerWorldTransform.translation_.y;
	matTrans.m[3][2] = playerWorldTransform.translation_.z;

	//�P�ʍs����
	playerWorldTransform.matWorld_.Reset();
	playerWorldTransform.matWorld_ *= matTrans;
	playerWorldTransform.TransferMatrix();
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

	matrotZ.m[0][0] = cos(playerWorldTransform.rotation_.z);
	matrotZ.m[0][1] = sin(playerWorldTransform.rotation_.z);
	matrotZ.m[1][0] = -sin(playerWorldTransform.rotation_.z);
	matrotZ.m[1][1] = cos(playerWorldTransform.rotation_.z);

	//�P�ʍs����
	matrotX.Reset();

	matrotX.m[1][1] = cos(playerWorldTransform.rotation_.x);
	matrotX.m[1][2] = sin(playerWorldTransform.rotation_.x);
	matrotX.m[2][1] = -sin(playerWorldTransform.rotation_.x);
	matrotX.m[2][2] = cos(playerWorldTransform.rotation_.x);

	//�P�ʍs����
	matrotY.Reset();

	matrotY.m[0][0] = cos(playerWorldTransform.rotation_.y);
	matrotY.m[0][2] = -sin(playerWorldTransform.rotation_.y);
	matrotY.m[2][0] = sin(playerWorldTransform.rotation_.y);
	matrotY.m[2][2] = cos(playerWorldTransform.rotation_.y);

	matRot = matrotZ;
	matRot *= matrotX;
	matRot *= matrotY;

	//�P�ʍs����
	playerWorldTransform.matWorld_.Reset();
	playerWorldTransform.matWorld_ *= matRot;
	UpdateMatrix();
	// worldTransform.TransferMatrix();
}

//�g�k
void Player::Scale() {}

//�g�k���s��]�S��
void Player::UpdateMatrix() {

	Matrix4 matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = MathUtility::Matrix4Identity();
	matScale.m[0][0] = playerWorldTransform.scale_.x;
	matScale.m[1][1] = playerWorldTransform.scale_.y;
	matScale.m[2][2] = playerWorldTransform.scale_.z;
	matScale.m[3][3] = 1;

	//��]
	matRot = MathUtility::Matrix4Identity();
	matRot = MathUtility::Matrix4RotationZ(playerWorldTransform.rotation_.z);
	matRot *= MathUtility::Matrix4RotationX(playerWorldTransform.rotation_.x);
	matRot *= MathUtility::Matrix4RotationY(playerWorldTransform.rotation_.y);

	//�ړ�
	matTrans = MathUtility::Matrix4Identity();
	matTrans = MathUtility::Matrix4Translation(
	  playerWorldTransform.translation_.x, playerWorldTransform.translation_.y,
	  playerWorldTransform.translation_.z);

	// ���[���h�s��̍���
	playerWorldTransform.matWorld_ = MathUtility::Matrix4Identity(); // �ό`�����Z�b�g
	playerWorldTransform.matWorld_ *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	playerWorldTransform.matWorld_ *= matRot;   // ���[���h�s��ɉ�]�𔽉f
	playerWorldTransform.matWorld_ *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	playerWorldTransform.TransferMatrix();
}

//�g�k���s��]�S��
void Player::UpdateMatrix(WorldTransform W) {

	Matrix4 matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = MathUtility::Matrix4Identity();
	matScale.m[0][0] = W.scale_.x;
	matScale.m[1][1] = W.scale_.y;
	matScale.m[2][2] = W.scale_.z;
	matScale.m[3][3] = 1;

	//��]
	matRot = MathUtility::Matrix4Identity();
	matRot = MathUtility::Matrix4RotationZ(W.rotation_.z);
	matRot *= MathUtility::Matrix4RotationX(W.rotation_.x);
	matRot *= MathUtility::Matrix4RotationY(W.rotation_.y);

	//�ړ�
	matTrans = MathUtility::Matrix4Identity();
	matTrans =
	  MathUtility::Matrix4Translation(W.translation_.x, W.translation_.y, W.translation_.z);

	// ���[���h�s��̍���
	W.matWorld_ = MathUtility::Matrix4Identity(); // �ό`�����Z�b�g
	W.matWorld_ *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	W.matWorld_ *= matRot;   // ���[���h�s��ɉ�]�𔽉f
	W.matWorld_ *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	W.TransferMatrix();
}

//�W�����v
void Player::Jump() {
	if (input->TriggerKey(DIK_SPACE)) {

		if (playerWorldTransform.translation_.y < -13.9) {

			Gravity = 0;

			jump = Maxjump;
		}
	}
}
