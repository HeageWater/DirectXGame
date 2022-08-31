#include "player.h"

#define PI 3.1415;

//ラディアン返す
float ReturnRadian(float n) {
	n *= PI;
	n /= 180;
	return n;
}

Vector3 Player::GetBulletWorldPosition() {
	Vector3 worldPos;

	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		worldPos.x = bullet->bulletW.translation_.x;
		worldPos.y = bullet->bulletW.translation_.y;
		worldPos.z = bullet->bulletW.translation_.z;
	}

	return worldPos;
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = playerW.translation_.x;
	worldPos.y = playerW.translation_.y;
	worldPos.z = playerW.translation_.z;

	return worldPos;
}

//初期化
void Player::Initialize(Model* model, Model* model2, uint32_t textureHandle) {
	assert(model);

	this->model = model;

	bulletmodel = model2;

	this->textureHandle = textureHandle;

	input = Input::GetInstance();
	debugText = DebugText::GetInstance();

	playerW.Initialize();

	playerW.translation_ = {0, 0, -10};
	playerW.scale_ = {3.0f, 3.0f, 3.0f};

	audio = Audio::GetInstance();

	Shot = audio->LoadWave("SE//shot.wav");
}

//描画
void Player::Draw(ViewProjection viewProjection) {
	model->Draw(playerW, viewProjection, textureHandle);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw(viewProjection);
	}
}

//更新
void Player::Update(WorldTransform enemy, ViewProjection viewProjection) {

	//デスフラグ
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

	//スピード
	const float speed = 0.3f;

	//最終的にransに足す値
	Vector3 move = {0, 0, 0};

	//コントローラー移動処理
	//コントローラー未接続なら抜ける
	/*if(!Input::GetInstance()->GetJoystickState(0, joyState)){
	   return;
	} else if (Input::GetInstance()->GetJoystickState(0, joyState)) {
	    move.x = (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed;
	    move.z = (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed;
	}*/

	//正面ベクトル
	centerDirection.z = viewProjection.target.z - viewProjection.eye.z;
	centerDirection.x = viewProjection.target.x - viewProjection.eye.x;
	centerDirection.y = viewProjection.target.y - viewProjection.eye.y;

	//長さを取得する
	centerV = sqrtf(
	  centerDirection.x * centerDirection.x + centerDirection.y * centerDirection.y +
	  centerDirection.z * centerDirection.z);

	//正規化
	center.z = centerDirection.z / centerV;
	center.x = centerDirection.x / centerV;
	center.y = centerDirection.y / centerV;

	//仮ベクトルに代入
//	rightV = temp.cross(center);

	// y軸移動
	if (input->PushKey(DIK_E)) {
		move.y = speed;
	} else if (input->PushKey(DIK_Q)) {
		move.y = -speed;
	}

	// x軸移動
	if (input->PushKey(DIK_D)) {
		move.x = speed * center.x;
	} else if (input->PushKey(DIK_A)) {
		move.x = -speed * center.x;
	}

	// z軸移動
	if (input->PushKey(DIK_W)) {
		move.z = speed * center.z;
	} else if (input->PushKey(DIK_S)) {
		move.z = -speed * center.z;
	}

	//回転
	if (input->PushKey(DIK_R)) {
		playerW.rotation_.y += 0.05f;
	}
	if (input->PushKey(DIK_T)) {
		playerW.rotation_.y -= 0.05f;
	}

	//Vector3 E = enemy.translation_;
	//Vector3 P = playerW.translation_;
	//E = E - P;
	//E.normalize();

	////ベクトルと行列の掛け算
	//playerW.rotation_.y = E.y;

	if (Gravity < MaxGravity) {
		Gravity += 0.02f;
	}

	//攻撃
	Attack(enemy);

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

	//移動限界
	const float kMoveLimitX = 48;
	const float kMoveLimitZ = 48;
	const float kMoveLimitY = 14;

	move = move.mat(move, playerW.matWorld_);

	playerW.translation_.x += move.x;
	playerW.translation_.y += move.y;
	playerW.translation_.z += move.z;

	//範囲を超えない処理
	playerW.translation_.x = max(playerW.translation_.x, -kMoveLimitX);
	playerW.translation_.x = min(playerW.translation_.x, +kMoveLimitX);
	playerW.translation_.y = max(playerW.translation_.y, -kMoveLimitY);
	playerW.translation_.y = min(playerW.translation_.y, +kMoveLimitY);
	playerW.translation_.z = max(playerW.translation_.z, -kMoveLimitZ);
	playerW.translation_.z = min(playerW.translation_.z, +kMoveLimitZ);

	//上全部
	UpdateMatrix();

	// debugText->SetPos(50, 70);
	// debugText->Printf("move:%f,%f,%f", move.x, move.y, move.z);
}

//攻撃
void Player::Attack(WorldTransform enemy) {
	if (input->TriggerKey(DIK_SPACE)) {

		if (audio->IsPlaying(Shot) != true) {

			audio->PlayWave(Shot, false, 0.05);
		}

		//弾速
		const float kBulletSpeed = -0.5f;

		Vector3 P = GetWorldPosition();
		Vector3 E = enemy.translation_;
		P = P - E;
		P.normalize();

		Vector3 velocity(kBulletSpeed, kBulletSpeed, kBulletSpeed);

		velocity.x *= P.x;
		velocity.y *= P.y;
		velocity.z *= P.z;

		//ベクトルと行列の掛け算
		velocity = velocity.mat(velocity, playerW.matWorld_);

		//弾生成
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(bulletmodel, playerW.translation_, velocity);

		//弾登録
		bullets.push_back(std::move(newBullet));
	}
}

//平行移動
void Player::Trans() {
	//平行移動行列宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = playerW.translation_.x;
	matTrans.m[3][1] = playerW.translation_.y;
	matTrans.m[3][2] = playerW.translation_.z;

	//単位行列代入
	playerW.matWorld_.Reset();
	playerW.matWorld_ *= matTrans;
	playerW.TransferMatrix();
}

//回転
void Player::Rota() {
	// Z軸回転行列
	Matrix4 matrotZ;

	//単位行列代入
	matrotZ.Reset();

	// X軸の回転
	Matrix4 matrotX;

	//単位行列代入
	matrotX.Reset();

	// Y軸の回転
	Matrix4 matrotY;

	//単位行列代入
	matrotY.Reset();

	//合成用回転行列
	Matrix4 matRot;

	//単位行列代入
	matrotZ.Reset();

	matrotZ.m[0][0] = cos(playerW.rotation_.z);
	matrotZ.m[0][1] = sin(playerW.rotation_.z);
	matrotZ.m[1][0] = -sin(playerW.rotation_.z);
	matrotZ.m[1][1] = cos(playerW.rotation_.z);

	//単位行列代入
	matrotX.Reset();

	matrotX.m[1][1] = cos(playerW.rotation_.x);
	matrotX.m[1][2] = sin(playerW.rotation_.x);
	matrotX.m[2][1] = -sin(playerW.rotation_.x);
	matrotX.m[2][2] = cos(playerW.rotation_.x);

	//単位行列代入
	matrotY.Reset();

	matrotY.m[0][0] = cos(playerW.rotation_.y);
	matrotY.m[0][2] = -sin(playerW.rotation_.y);
	matrotY.m[2][0] = sin(playerW.rotation_.y);
	matrotY.m[2][2] = cos(playerW.rotation_.y);

	matRot = matrotZ;
	matRot *= matrotX;
	matRot *= matrotY;

	//単位行列代入
	playerW.matWorld_.Reset();
	playerW.matWorld_ *= matRot;
	UpdateMatrix();
	// worldTransform.TransferMatrix();
}

//拡縮
void Player::Scale() {}

//拡縮平行回転全部
void Player::UpdateMatrix() {

	Matrix4 matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = MathUtility::Matrix4Identity();
	matScale.m[0][0] = playerW.scale_.x;
	matScale.m[1][1] = playerW.scale_.y;
	matScale.m[2][2] = playerW.scale_.z;
	matScale.m[3][3] = 1;

	//回転
	matRot = MathUtility::Matrix4Identity();
	matRot = MathUtility::Matrix4RotationZ(playerW.rotation_.z);
	matRot *= MathUtility::Matrix4RotationX(playerW.rotation_.x);
	matRot *= MathUtility::Matrix4RotationY(playerW.rotation_.y);

	//移動
	matTrans = MathUtility::Matrix4Identity();
	matTrans = MathUtility::Matrix4Translation(
	  playerW.translation_.x, playerW.translation_.y, playerW.translation_.z);

	// ワールド行列の合成
	playerW.matWorld_ = MathUtility::Matrix4Identity(); // 変形をリセット
	playerW.matWorld_ *= matScale; // ワールド行列にスケーリングを反映
	playerW.matWorld_ *= matRot;   // ワールド行列に回転を反映
	playerW.matWorld_ *= matTrans; // ワールド行列に平行移動を反映

	playerW.TransferMatrix();
}

//当たった時の処理
void Player::OnCollision() {}

//ジャンプ
void Player::Jump() {
	if (input->TriggerKey(DIK_J)) {
		Gravity = 0;

		jump = Maxjump;
	}

	/*if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
	    Gravity = 0;

	    jump = Maxjump;
	}*/
}

//ダッシュ
void Player::Dush() {
	if (input->PushKey(DIK_K)) {
		if (dush_flg != true) {
			dush_flg = true;
			dushcount = 10;
		}
	}

	/*if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
	    if (dush_flg != true) {
	        dush_flg = true;
	        dushcount = 10;
	    }
	}*/

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

// Vector3 Player::GetBulletWorldPosition() {
//	Vector3 a;
//
// }

//説明
// void moveee() {
//	////Z軸の回転
//	// worldTransform_.rotation_ = { 0.0f,0.0f,ReturnRadian(45.0f)};
//
//	////Z軸回転行列
//	// Matrix4 matrotZ;
//
//	////単位行列代入
//	// matrotZ.Reset();
//
//	// matrotZ.m[0][0] = cos(worldTransform_.rotation_.z);
//	// matrotZ.m[0][1] = sin(worldTransform_.rotation_.z);
//	// matrotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
//	// matrotZ.m[1][1] = cos(worldTransform_.rotation_.z);
//
//	////単位行列代入
//	// worldTransform_.matWorld_.Reset();
//
//	// worldTransform_.matWorld_ *= matrotZ;
//
//	// worldTransform_.TransferMatrix();
//
//	////X軸の回転
//	// worldTransform_.rotation_ = {ReturnRadian(45.0f) , 0.0f,0.0f };
//
//	// Matrix4 matrotX;
//
//	////単位行列代入
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
//	////Y軸の回転
//	// worldTransform_.rotation_ = { 0.0f,ReturnRadian(45.0f) ,0.0f };
//
//	// Matrix4 matrotY;
//
//	////単位行列代入
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
//	////合成用回転行列
//	// Matrix4 matRot;
//
//	////各軸の回転行列
//	// Matrix4 matrotX, matrotY, matrotZ;
//
//	////単位行列代入
//	// matrotZ.Reset();
//
//	// matrotZ.m[0][0] = cos(worldTransform_.rotation_.z);
//	// matrotZ.m[0][1] = sin(worldTransform_.rotation_.z);
//	// matrotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
//	// matrotZ.m[1][1] = cos(worldTransform_.rotation_.z);
//
//	////単位行列代入
//	// matrotX.Reset();
//
//	// matrotX.m[1][1] = cos(worldTransform_.rotation_.x);
//	// matrotX.m[1][2] = sin(worldTransform_.rotation_.x);
//	// matrotX.m[2][1] = -sin(worldTransform_.rotation_.x);
//	// matrotX.m[2][2] = cos(worldTransform_.rotation_.x);
//
//	////単位行列代入
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
//	////単位行列代入
//	////worldTransform_.matWorld_.Reset();
//	// worldTransform_.matWorld_ *= matRot;
//	// worldTransform_.TransferMatrix();
//
//	////平行移動
//	// worldTransform_.translation_ = { 5.0f,0.0f,0.0f };
//
//	////平行移動行列宣言
//	// Matrix4 matTrans = MathUtility::Matrix4Identity();
//
//	// matTrans.m[3][0] = worldTransform_.translation_.x;
//	// matTrans.m[3][1] = worldTransform_.translation_.y;
//	// matTrans.m[3][2] = worldTransform_.translation_.z;
//
//	////単位行列代入
//	////worldTransform_.matWorld_.Reset();
//	// worldTransform_.matWorld_ *= matTrans;
//	// worldTransform_.TransferMatrix();
//}

//コントローラー未接続なら抜ける
// if(!Input::GetInstance()->GetJoystickState(0, joyState){
// return;
//}
