#include "player.h"

#define PI 3.1415;

//���f�B�A���Ԃ�
float ReturnRadian(float n) {
	n *= PI;
	n /= 180;
	return n;
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = playerW.translation_.x;
	worldPos.y = playerW.translation_.y;
	worldPos.z = playerW.translation_.z;

	return worldPos;
}

//������
void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model = model;

	this->textureHandle = textureHandle;

	input = Input::GetInstance();
	debugText = DebugText::GetInstance();

	playerW.Initialize();

	playerW.translation_ = {0, 0, -10};
	playerW.scale_ = {3.0f, 3.0f, 3.0f};
}

//�`��
void Player::Draw(ViewProjection viewProjection) {
	model->Draw(playerW, viewProjection, textureHandle);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw(viewProjection);
	}
}

//�X�V
void Player::Update() {

	//�f�X�t���O
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

	//�X�s�[�h
	const float speed = 0.3f;

	//�ŏI�I��rans�ɑ����l
	Vector3 move = {0, 0, 0};

	// y���ړ�
	if (input->PushKey(DIK_E)) {
		move.y = speed;
	} else if (input->PushKey(DIK_Q)) {
		move.y = -speed;
	}

	// x���ړ�
	if (input->PushKey(DIK_D)) {
		move.x = speed;
	} else if (input->PushKey(DIK_A)) {
		move.x = -speed;
	}

	// z���ړ�
	if (input->PushKey(DIK_W)) {
		move.z = speed;
	} else if (input->PushKey(DIK_S)) {
		move.z = -speed;
	}

	//��]
	if (input->PushKey(DIK_R)) {
		playerW.rotation_.y += 0.05f;
	}
	if (input->PushKey(DIK_T)) {
		playerW.rotation_.y -= 0.05f;
	}

	if (Gravity < MaxGravity) {
		Gravity += 0.02f;
	}

	//�U��
	Attack();

	Jump();

	Dush();

	if (jump > 0) {
		playerW.translation_.y += jump;

		jump -= 0.1f;
	} else {
		playerW.translation_.y -= Gravity;
	}

	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Update();
	}

	//�ړ����E
	const float kMoveLimitX = 48;
	const float kMoveLimitY = 14;

	move = move.mat(move, playerW.matWorld_);

	playerW.translation_.x += move.x;
	playerW.translation_.y += move.y;
	playerW.translation_.z += move.z;

	//�͈͂𒴂��Ȃ�����
	playerW.translation_.x = max(playerW.translation_.x, -kMoveLimitX);
	playerW.translation_.x = min(playerW.translation_.x, +kMoveLimitX);
	playerW.translation_.y = max(playerW.translation_.y, -kMoveLimitY);
	playerW.translation_.y = min(playerW.translation_.y, +kMoveLimitY);

	//��S��
	UpdateMatrix();

	debugText->SetPos(50, 70);
	debugText->Printf("move:%f,%f,%f", move.x, move.y, move.z);
}

//�U��
void Player::Attack() {
	if (input->TriggerKey(DIK_SPACE)) {

		//�e��
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//�x�N�g���ƍs��̊|���Z
		velocity = velocity.mat(velocity, playerW.matWorld_);

		//�e����
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model, playerW.translation_, velocity);

		//�e�o�^
		bullets.push_back(std::move(newBullet));
	}
}

//���s�ړ�
void Player::Trans() {
	//���s�ړ��s��錾
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = playerW.translation_.x;
	matTrans.m[3][1] = playerW.translation_.y;
	matTrans.m[3][2] = playerW.translation_.z;

	//�P�ʍs����
	playerW.matWorld_.Reset();
	playerW.matWorld_ *= matTrans;
	playerW.TransferMatrix();
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

	matrotZ.m[0][0] = cos(playerW.rotation_.z);
	matrotZ.m[0][1] = sin(playerW.rotation_.z);
	matrotZ.m[1][0] = -sin(playerW.rotation_.z);
	matrotZ.m[1][1] = cos(playerW.rotation_.z);

	//�P�ʍs����
	matrotX.Reset();

	matrotX.m[1][1] = cos(playerW.rotation_.x);
	matrotX.m[1][2] = sin(playerW.rotation_.x);
	matrotX.m[2][1] = -sin(playerW.rotation_.x);
	matrotX.m[2][2] = cos(playerW.rotation_.x);

	//�P�ʍs����
	matrotY.Reset();

	matrotY.m[0][0] = cos(playerW.rotation_.y);
	matrotY.m[0][2] = -sin(playerW.rotation_.y);
	matrotY.m[2][0] = sin(playerW.rotation_.y);
	matrotY.m[2][2] = cos(playerW.rotation_.y);

	matRot = matrotZ;
	matRot *= matrotX;
	matRot *= matrotY;

	//�P�ʍs����
	playerW.matWorld_.Reset();
	playerW.matWorld_ *= matRot;
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
	matScale.m[0][0] = playerW.scale_.x;
	matScale.m[1][1] = playerW.scale_.y;
	matScale.m[2][2] = playerW.scale_.z;
	matScale.m[3][3] = 1;

	//��]
	matRot = MathUtility::Matrix4Identity();
	matRot = MathUtility::Matrix4RotationZ(playerW.rotation_.z);
	matRot *= MathUtility::Matrix4RotationX(playerW.rotation_.x);
	matRot *= MathUtility::Matrix4RotationY(playerW.rotation_.y);

	//�ړ�
	matTrans = MathUtility::Matrix4Identity();
	matTrans = MathUtility::Matrix4Translation(
	  playerW.translation_.x, playerW.translation_.y, playerW.translation_.z);

	// ���[���h�s��̍���
	playerW.matWorld_ = MathUtility::Matrix4Identity(); // �ό`�����Z�b�g
	playerW.matWorld_ *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	playerW.matWorld_ *= matRot;   // ���[���h�s��ɉ�]�𔽉f
	playerW.matWorld_ *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	playerW.TransferMatrix();
}

void Player::OnCollision() {}

void Player::Jump() {
	if (input->TriggerKey(DIK_J)) {
		Gravity = 0;

		jump = Maxjump;
	}
}

void Player::Dush() {
	if (input->TriggerKey(DIK_K)) {
		if (dush_flg != true) {
			dush_flg = true;
			dushcount = 10;
		}
	}

	if (dush_flg == true) {

		Vector3 move = {0.0f, 0.0f, 1.0f};
		move = move.mat(move, playerW.matWorld_);

		playerW.translation_.x += move.x;
		playerW.translation_.y += move.y;
		playerW.translation_.z += move.z;

		dushcount--;
	}

	if (dushcount < 0) {
		dush_flg = false;
	}
}

//����
// void moveee() {
//	////Z���̉�]
//	// worldTransform_.rotation_ = { 0.0f,0.0f,ReturnRadian(45.0f)};
//
//	////Z����]�s��
//	// Matrix4 matrotZ;
//
//	////�P�ʍs����
//	// matrotZ.Reset();
//
//	// matrotZ.m[0][0] = cos(worldTransform_.rotation_.z);
//	// matrotZ.m[0][1] = sin(worldTransform_.rotation_.z);
//	// matrotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
//	// matrotZ.m[1][1] = cos(worldTransform_.rotation_.z);
//
//	////�P�ʍs����
//	// worldTransform_.matWorld_.Reset();
//
//	// worldTransform_.matWorld_ *= matrotZ;
//
//	// worldTransform_.TransferMatrix();
//
//	////X���̉�]
//	// worldTransform_.rotation_ = {ReturnRadian(45.0f) , 0.0f,0.0f };
//
//	// Matrix4 matrotX;
//
//	////�P�ʍs����
//	// matrotX.Reset();
//
//	// matrotX.m[1][1] = cos(worldTransform_.rotation_.x);
//	// matrotX.m[1][2] = sin(worldTransform_.rotation_.x);
//	// matrotX.m[2][1] = -sin(worldTransform_.rotation_.x);
//	// matrotX.m[2][2] = cos(worldTransform_.rotation_.x);
//
//	// worldTransform_.matWorld_ *= matrotX;
//
//	// worldTransform_.TransferMatrix();
//
//	////Y���̉�]
//	// worldTransform_.rotation_ = { 0.0f,ReturnRadian(45.0f) ,0.0f };
//
//	// Matrix4 matrotY;
//
//	////�P�ʍs����
//	// matrotY.Reset();
//
//	// matrotY.m[0][0] = cos(worldTransform_.rotation_.y);
//	// matrotY.m[0][2] = -sin(worldTransform_.rotation_.y);
//	// matrotY.m[2][0] = sin(worldTransform_.rotation_.y);
//	// matrotY.m[2][2] = cos(worldTransform_.rotation_.y);
//
//	// worldTransform_.matWorld_ *= matrotY;
//
//	// worldTransform_.TransferMatrix();
//
//	////x,y,z
//	// worldTransform_.rotation_ = { ReturnRadian(45.0f) ,ReturnRadian(45.0f) ,ReturnRadian(45.0f)
//	// };
//
//	////�����p��]�s��
//	// Matrix4 matRot;
//
//	////�e���̉�]�s��
//	// Matrix4 matrotX, matrotY, matrotZ;
//
//	////�P�ʍs����
//	// matrotZ.Reset();
//
//	// matrotZ.m[0][0] = cos(worldTransform_.rotation_.z);
//	// matrotZ.m[0][1] = sin(worldTransform_.rotation_.z);
//	// matrotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
//	// matrotZ.m[1][1] = cos(worldTransform_.rotation_.z);
//
//	////�P�ʍs����
//	// matrotX.Reset();
//
//	// matrotX.m[1][1] = cos(worldTransform_.rotation_.x);
//	// matrotX.m[1][2] = sin(worldTransform_.rotation_.x);
//	// matrotX.m[2][1] = -sin(worldTransform_.rotation_.x);
//	// matrotX.m[2][2] = cos(worldTransform_.rotation_.x);
//
//	////�P�ʍs����
//	// matrotY.Reset();
//
//	// matrotY.m[0][0] = cos(worldTransform_.rotation_.y);
//	// matrotY.m[0][2] = -sin(worldTransform_.rotation_.y);
//	// matrotY.m[2][0] = sin(worldTransform_.rotation_.y);
//	// matrotY.m[2][2] = cos(worldTransform_.rotation_.y);
//
//	// matRot = matrotZ;
//	// matRot *= matrotX;
//	// matRot *= matrotY;
//
//	////�P�ʍs����
//	////worldTransform_.matWorld_.Reset();
//	// worldTransform_.matWorld_ *= matRot;
//	// worldTransform_.TransferMatrix();
//
//	////���s�ړ�
//	// worldTransform_.translation_ = { 5.0f,0.0f,0.0f };
//
//	////���s�ړ��s��錾
//	// Matrix4 matTrans = MathUtility::Matrix4Identity();
//
//	// matTrans.m[3][0] = worldTransform_.translation_.x;
//	// matTrans.m[3][1] = worldTransform_.translation_.y;
//	// matTrans.m[3][2] = worldTransform_.translation_.z;
//
//	////�P�ʍs����
//	////worldTransform_.matWorld_.Reset();
//	// worldTransform_.matWorld_ *= matTrans;
//	// worldTransform_.TransferMatrix();
//}