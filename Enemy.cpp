#include "Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model = model;

	this->textureHandle = textureHandle;

	EnemyW.Initialize();

	EnemyW.translation_ = {0,5,50};
}

void Enemy::Draw(ViewProjection viewProjection) {
	model->Draw(EnemyW, viewProjection, textureHandle);
}

void Enemy::Update() {
	//�X�s�[�h
	const float speed = 0.3f;

	//�ŏI�I��rans�ɑ����l
	Vector3 move = {0, 0, -speed};

	EnemyW.translation_ += move;

	//��S��
	UpdateMatrix();
}

//�g�k���s��]�S��
void Enemy::UpdateMatrix() {

	Matrix4 matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = MathUtility::Matrix4Identity();
	matScale.m[0][0] = EnemyW.scale_.x;
	matScale.m[1][1] = EnemyW.scale_.y;
	matScale.m[2][2] = EnemyW.scale_.z;
	matScale.m[3][3] = 1;

	//��]
	matRot = MathUtility::Matrix4Identity();
	matRot = MathUtility::Matrix4RotationZ(EnemyW.rotation_.z);
	matRot *= MathUtility::Matrix4RotationX(EnemyW.rotation_.x);
	matRot *= MathUtility::Matrix4RotationY(EnemyW.rotation_.y);

	//�ړ�
	matTrans = MathUtility::Matrix4Identity();
	matTrans = MathUtility::Matrix4Translation(
	  EnemyW.translation_.x, EnemyW.translation_.y, EnemyW.translation_.z);

	// ���[���h�s��̍���
	EnemyW.matWorld_ = MathUtility::Matrix4Identity(); // �ό`�����Z�b�g
	EnemyW.matWorld_ *= matScale;  // ���[���h�s��ɃX�P�[�����O�𔽉f
	EnemyW.matWorld_ *= matRot;   // ���[���h�s��ɉ�]�𔽉f
	EnemyW.matWorld_ *= matTrans;      // ���[���h�s��ɕ��s�ړ��𔽉f

	EnemyW.TransferMatrix();
}
