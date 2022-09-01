#include "Enemy.h"

void Enemy::Reset() {
	EnemyW.translation_ = {0, 5, 15};

	EnemyW.scale_ = {2.0f, 2.0f, 2.0};

	hp = Maxhp;

	damege = 10;

	muteki = 0;

	isDead_ = false;

	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->isDead_ = true;
	}
}

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

	input = Input::GetInstance();

	EnemyW.Initialize();

	EnemyW.translation_ = {0, 5, 15};
	EnemyW.scale_ = {2.0f, 2.0f, 2.0};

	audio = Audio::GetInstance();

	Shot = audio->LoadWave("SE//Eshot.wav");

	hp = Maxhp;

	muteki = 0;

	for (int i = 0; i < 10; i++) {

		pate[i].translation_ = {1, 1, 1};
		pate[i].scale_ = {1, 1, 1};
	}
}

void Enemy::Draw(ViewProjection viewProjection) {
	if (isDead_ != true) {
		if (muteki % 2 == 0) {
			model->Draw(EnemyW, viewProjection, textureHandle);
		}

		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
			bullet->Draw(viewProjection);
		}
	}

	if (isDead_ != false && pate_flg != true) {
		for (int i = 0; i < 10; i++) {

			pate[i].Initialize();

			pate[i].translation_ = EnemyW.translation_;

			pate[i].scale_ = {1, 1, 1};
		}
		pate_flg = true;
	}

	if (pate_flg) {

		pate[0].translation_.x += 1;
		pate[1].translation_.x -= 1;
		pate[2].translation_.y += 1;
		pate[3].translation_.y -= 1;
		pate[4].translation_.z += 1;
		pate[5].translation_.z -= 1;

		pate[6].translation_.x -= 1;
		pate[6].translation_.z -= 1;

		pate[7].translation_.x += 1;
		pate[7].translation_.z += 1;

		pate[8].translation_.x += 1;
		pate[8].translation_.z -= 1;

		pate[9].translation_.x -= 1;
		pate[9].translation_.z += 1;

		for (int i = 0; i < 10; i++) {

			UpdateMatrix(pate[i]);

			model->Draw(pate[i], viewProjection, textureHandle);
		}
	}
}

void Enemy::Update(WorldTransform play, Model* bulletmodel) {

	if (isDead_ != true) {

		Ktimer--;

		if (input->TriggerKey(DIK_P)) {
			damege = Maxhp;
		}

		if (muteki > 0) {
			muteki--;
		}

		if (Ktimer < 0) {
			if (audio->IsPlaying(Shot) != true) {

				audio->PlayWave(Shot, false, 0.1);
			}
			Fire(play, bulletmodel);
			Ktimer = Kfire;

			a = GetRandom();

			if (a > 5) {
				phase_ = Phase::Stay;
			} else {
				phase_ = Phase::Move;
			}
		}

		//スピード
		const float speed = 0.3f;

		//最終的にransに足す値
		Vector3 move = {0, 0, 0};

		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
			bullet->Update();
		}

		switch (phase_) {
		case Phase::Move:

			if (Ktimer < 3) {
				a = GetRandom();

				move.x = a - 5;

				a = GetRandom();

				move.z = a - 5;
			}

			//重力
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

			if (a == 4 || a == -4) {
				Jump();
				a = 0;
			}

			if (a == -2 || a == 2) {
				Dush();
				a = 0;
			}

			//範囲を超えない処理
			EnemyW.translation_.x = max(EnemyW.translation_.x, -kMoveLimitX);
			EnemyW.translation_.x = min(EnemyW.translation_.x, +kMoveLimitX);
			EnemyW.translation_.y = max(EnemyW.translation_.y, -kMoveLimitY);
			EnemyW.translation_.y = min(EnemyW.translation_.y, +kMoveLimitY);
			EnemyW.translation_.z = max(EnemyW.translation_.z, -kMoveLimitZ);
			EnemyW.translation_.z = min(EnemyW.translation_.z, +kMoveLimitZ);
			//上全部
			UpdateMatrix(EnemyW);
			break;
		case Phase::Stay:

			//上全部
			UpdateMatrix(EnemyW);
			break;
		default:
			EnemyW.translation_ += move;

			//上全部
			UpdateMatrix(EnemyW);
			break;
		}
	}
}

//拡縮平行回転全部
void Enemy::UpdateMatrix(WorldTransform W) {

	Matrix4 matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = MathUtility::Matrix4Identity();
	matScale.m[0][0] = W.scale_.x;
	matScale.m[1][1] = W.scale_.y;
	matScale.m[2][2] = W.scale_.z;
	matScale.m[3][3] = 1;

	//回転
	matRot = MathUtility::Matrix4Identity();
	matRot = MathUtility::Matrix4RotationZ(W.rotation_.z);
	matRot *= MathUtility::Matrix4RotationX(W.rotation_.x);
	matRot *= MathUtility::Matrix4RotationY(W.rotation_.y);

	//移動
	matTrans = MathUtility::Matrix4Identity();
	matTrans =
	  MathUtility::Matrix4Translation(W.translation_.x, W.translation_.y, W.translation_.z);

	// ワールド行列の合成
	W.matWorld_ = MathUtility::Matrix4Identity(); // 変形をリセット
	W.matWorld_ *= matScale; // ワールド行列にスケーリングを反映
	W.matWorld_ *= matRot;   // ワールド行列に回転を反映
	W.matWorld_ *= matTrans; // ワールド行列に平行移動を反映

	W.TransferMatrix();
}

void Enemy::Fire(WorldTransform play, Model* bulletmodel) {
	assert(player_);

	//弾速
	const float kBulletSpeed = -0.5f;

	Vector3 E = GetWorldPosition();
	Vector3 P = play.translation_;
	E = E - P;
	E.normalize();

	Vector3 velocity(kBulletSpeed, kBulletSpeed, kBulletSpeed);

	velocity.x *= E.x;
	velocity.y *= E.y;
	velocity.z *= E.z;

	//ベクトルと行列の掛け算
	velocity = velocity.mat(velocity, EnemyW.matWorld_);

	//弾生成
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(bulletmodel, EnemyW.translation_, velocity);

	//弾登録
	bullets.push_back(std::move(newBullet));
}

void Enemy::OnCollision() {
	if (muteki <= 1) {
		hp -= damege;
		muteki = 30;
	}

	if (hp <= 0) {
		isDead_ = true;
	}
}

//ジャンプ
void Enemy::Jump() {
	if (EnemyW.translation_.y < -13.9) {

		Gravity = 0;

		jump = Maxjump;
	}
}

//ダッシュ
void Enemy::Dush() {
	if (dush_flg != true) {
		dush_flg = true;
		dushcount = 10;
	}

	if (dush_flg == true) {

		Vector3 move = {0.1f, 0.1f, 0.1f};
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

float Enemy::GetRandom() {
	std::random_device seed_gen;

	std::mt19937_64 engine(seed_gen());

	std::uniform_real_distribution<float> dist(0, 10);

	value = dist(engine);

	return value;
}