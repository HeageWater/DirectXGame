#include "playerbullet.h"

Vector3 PlayerBullet::GetPosition() {
	Vector3 worldPos;

	worldPos.x = bulletW.translation_.x;
	worldPos.y = bulletW.translation_.y;
	worldPos.z = bulletW.translation_.z;

	return worldPos;
}

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity_) {

	// NULL�|�C���^�`�F�b�N
	assert(model);

	//���f��
	this->model = model;

	//�摜�f�[�^
	textureHandle = TextureManager::Load("kyu//kyu.png");

	//������
	bulletW.Initialize();

	//�ꏊ���
	bulletW.translation_ = position;

	bulletW.scale_ = {2.0f,2.0f,2.0f};

	//���x���
	velocity = velocity_;
}

void PlayerBullet::Update() {

	bulletW.translation_ += velocity;

	if (--deathTimer <= 0) {

		isDead_ = true;
	}

	// worldTransform�X�V
	//�X�P-�����O�s��̐錾
	Matrix4 matScale;

	//�P�ʍs�����
	matScale.Reset();
	//�X�P-�����O�{�����s��ɐݒ肷��
	matScale.m[0][0] = bulletW.scale_.x;
	matScale.m[1][1] = bulletW.scale_.y;
	matScale.m[2][2] = bulletW.scale_.z;

	//�P�ʍs�����
	bulletW.matWorld_.Reset();

	// matScale���|���đ��
	bulletW.matWorld_.m[0][0] *= matScale.m[0][0];
	bulletW.matWorld_.m[1][1] *= matScale.m[1][1];
	bulletW.matWorld_.m[2][2] *= matScale.m[2][2];

	//�����p��]�s��
	Matrix4 matRot;

	//�e���̉�]�s��
	Matrix4 matrotX, matrotY, matrotZ;

	//�P�ʍs����
	matrotZ.Reset();

	matrotZ.m[0][0] = cos(bulletW.rotation_.z);
	matrotZ.m[0][1] = sin(bulletW.rotation_.z);
	matrotZ.m[1][0] = -sin(bulletW.rotation_.z);
	matrotZ.m[1][1] = cos(bulletW.rotation_.z);

	//�P�ʍs����
	matrotX.Reset();

	matrotX.m[1][1] = cos(bulletW.rotation_.x);
	matrotX.m[1][2] = sin(bulletW.rotation_.x);
	matrotX.m[2][1] = -sin(bulletW.rotation_.x);
	matrotX.m[2][2] = cos(bulletW.rotation_.x);

	//�P�ʍs����
	matrotY.Reset();

	matrotY.m[0][0] = cos(bulletW.rotation_.y);
	matrotY.m[0][2] = -sin(bulletW.rotation_.y);
	matrotY.m[2][0] = sin(bulletW.rotation_.y);
	matrotY.m[2][2] = cos(bulletW.rotation_.y);

	matRot = matrotZ;
	matRot *= matrotX;
	matRot *= matrotY;

	//�P�ʍs����
	// worldTransform_.matWorld_.Reset();
	bulletW.matWorld_ *= matRot;
	bulletW.TransferMatrix();

	//���s�ړ��s��錾
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = bulletW.translation_.x;
	matTrans.m[3][1] = bulletW.translation_.y;
	matTrans.m[3][2] = bulletW.translation_.z;

	//�P�ʍs����
	// worldTransform_.matWorld_.Reset();
	bulletW.matWorld_ *= matTrans;
	bulletW.TransferMatrix();
}

//�e�`��
void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model->Draw(bulletW, viewProjection, textureHandle);
}

//�����蔻��
void PlayerBullet::OnCollision() { isDead_ = true; }