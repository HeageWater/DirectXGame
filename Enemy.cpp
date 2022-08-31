#include "Enemy.h"

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = EnemyW.translation_.x;
	worldPos.y = EnemyW.translation_.y;
	worldPos.z = EnemyW.translation_.z;

	return worldPos;
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model = model;

	this->textureHandle = textureHandle;

	EnemyW.Initialize();

	EnemyW.translation_ = {0, 5, -10};
	EnemyW.scale_ = {2.0f, 2.0f, 2.0};
}

void Enemy::Draw(ViewProjection viewProjection) {
	if (isDead_ != true) {
		model->Draw(EnemyW, viewProjection, textureHandle);

		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
			bullet->Draw(viewProjection);
		}
	}
}

void Enemy::Update(WorldTransform play, Model* bulletmodel) {

	if (isDead_ != true) {

		Ktimer--;

		if (Ktimer < 0) {
			Fire(play, bulletmodel);
			Ktimer = Kfire;
		}

		//�X�s�[�h
		const float speed = 0.3f;

		//�ŏI�I��rans�ɑ����l
		Vector3 move = {0, 0, -speed};

		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
			bullet->Update();
		}

		switch (phase_) {
		case Phase::Move:

			//�d��
			if (Gravity < MaxGravity) {
				Gravity += 0.02f;
			}

			if (jump > 0) {
				EnemyW.translation_.y += jump;

				jump -= 0.1f;
			} else {
				EnemyW.translation_.y -= Gravity;
			}

			move = move.mat(move, EnemyW.matWorld_);

			EnemyW.translation_.x += move.x;
			EnemyW.translation_.y += move.y;
			EnemyW.translation_.z += move.z;

			//�͈͂𒴂��Ȃ�����
			EnemyW.translation_.x = max(EnemyW.translation_.x, -kMoveLimitX);
			EnemyW.translation_.x = min(EnemyW.translation_.x, +kMoveLimitX);
			EnemyW.translation_.y = max(EnemyW.translation_.y, -kMoveLimitY);
			EnemyW.translation_.y = min(EnemyW.translation_.y, +kMoveLimitY);
			EnemyW.translation_.z = max(EnemyW.translation_.z, -kMoveLimitZ);
			EnemyW.translation_.z = min(EnemyW.translation_.z, +kMoveLimitZ);
			//��S��
			UpdateMatrix();
			break;
		case Phase::Stay:

			//��S��
			UpdateMatrix();
			break;
		default:
			EnemyW.translation_ += move;

			//��S��
			UpdateMatrix();
			break;
		}
	}
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
	EnemyW.matWorld_ *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	EnemyW.matWorld_ *= matRot;   // ���[���h�s��ɉ�]�𔽉f
	EnemyW.matWorld_ *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	EnemyW.TransferMatrix();
}

void Enemy::Fire(WorldTransform play, Model* bulletmodel) {
	assert(player_);

	//�e��
	const float kBulletSpeed = -0.5f;

	Vector3 E = GetWorldPosition();
	Vector3 P = play.translation_;
	E = E - P;
	E.normalize();

	Vector3 velocity(kBulletSpeed, kBulletSpeed, kBulletSpeed);

	velocity.x *= E.x;
	velocity.y *= E.y;
	velocity.z *= E.z;

	//�x�N�g���ƍs��̊|���Z
	velocity = velocity.mat(velocity, EnemyW.matWorld_);

	//�e����
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(bulletmodel, EnemyW.translation_, velocity);

	//�e�o�^
	bullets.push_back(std::move(newBullet));
}

void Enemy::OnCollision() { isDead_ = true; }

//�W�����v
void Enemy::Jump() {
	Gravity = 0;

	jump = Maxjump;
}

//�_�b�V��
void Enemy::Dush() {
	if (dush_flg != true) {
		dush_flg = true;
		dushcount = 10;
	}

	if (dush_flg == true) {

		Vector3 move = {0.0f, 0.0f, 1.0f};
		move = move.mat(move, EnemyW.matWorld_);

		EnemyW.translation_.x += move.x;
		EnemyW.translation_.y += move.y;
		EnemyW.translation_.z += move.z;

		dushcount--;
	}

	if (dushcount < 0) {
		dush_flg = false;
	}
}