#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

#define PI = 3.1415;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete sprite_;
}

WorldTransform cross(WorldTransform v, WorldTransform s) {

	WorldTransform a;
	a.translation_.x = v.translation_.y * s.translation_.z - v.translation_.y * s.translation_.y;
	a.translation_.y = v.translation_.z * s.translation_.x - v.translation_.z * s.translation_.z;
	a.translation_.z = v.translation_.x * s.translation_.y - v.translation_.x * s.translation_.x;

	return a;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("cube//cube.jpg");

	model_ = Model::Create();
	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	worldTransform_.scale_ = {3.0f, 3.0f, 3.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	tarW.scale_ = {2.0f, 2.0f, 2.0f};
	tarW.rotation_ = {0.0f, 0.0f, 0.0f};
	tarW.translation_ = {5.0f, 0.0f, 0.0f};

	worldTransform2_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform2_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform2_.translation_ = {0.0f, 0.0f, 0.0f};

	bowW.scale_ = {5.0f, 1.0f, 1.0f};
	bowW.rotation_ = {0.0f, 0.0f, 0.0f};
	bowW.translation_ = {0.0f, 0.0f, 0.0f};

	worldTransform_.Initialize();
	worldTransform2_.Initialize();
	viewProjection_.Initialize();
	viewProjection2_.Initialize();

	bowW.Initialize();
	bowT.Initialize();

	tarW.Initialize();
	tarV.Initialize();

	//物体の移動ベクトル
	direction = {0, 0, 0};
	bowD = {0, 0, 0};

	viewProjection_.eye.y = 15.0f;
}

void GameScene::Update() {

	//正面への移動速度
	const float kTSpeed = 0.3f;

	//正面への移動速度
	const float kRSpeed = 0.1f;

	if (input_->PushKey(DIK_N)) {
		tarW.translation_.z += kTSpeed;
	} else if (input_->PushKey(DIK_M)) {
		tarW.translation_.z -= kTSpeed;
	}
	if (input_->PushKey(DIK_B)) {
		tarW.translation_.x += kTSpeed;
	} else if (input_->PushKey(DIK_V)) {
		tarW.translation_.x -= kTSpeed;
	}

	if (input_->PushKey(DIK_Q)) {
		worldTransform_.translation_.z += kTSpeed;
		worldTransform2_.translation_.z += kTSpeed;
	} else if (input_->PushKey(DIK_E)) {
		worldTransform_.translation_.z -= kTSpeed;
		worldTransform2_.translation_.z -= kTSpeed;
	}

	if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRSpeed;
		worldTransform2_.rotation_.y += kRSpeed;

	} else if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRSpeed;
		worldTransform2_.rotation_.y -= kRSpeed;
	}

	direction.x = (cos(worldTransform_.rotation_.y) - sin(worldTransform_.rotation_.z));
	direction.z = (cos(worldTransform_.rotation_.x) - sin(worldTransform_.rotation_.y)) - 1;

	// 2π超えたら0にする
	worldTransform_.rotation_.y = fmodf(worldTransform_.rotation_.y, XM_2PI);

	if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.x += kTSpeed * direction.x;
		worldTransform2_.translation_.x += kTSpeed * direction.x;
	} else if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.x -= kTSpeed * direction.x;
		worldTransform2_.translation_.x -= kTSpeed * direction.x;
	}

	if (input_->PushKey(DIK_UP)) {
		bowW.translation_.z += kTSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		bowW.translation_.z -= kTSpeed;
	}

	if (input_->PushKey(DIK_RIGHT)) {
		bowW.rotation_.y += kRSpeed;
	} else if (input_->PushKey(DIK_LEFT)) {
		bowW.rotation_.y -= kRSpeed;
	}

	if (input_->TriggerKey(DIK_Z)) {
		worldTransform_.rotation_.y = tarW.translation_.y;
		worldTransform_.rotation_.z = tarW.translation_.z;
	}

	if (input_->TriggerKey(DIK_X)) {
		tarW.translation_.x = 5;
		tarW.translation_.y = 5;
		tarW.translation_.z = 5;
	}

	if (input_->TriggerKey(DIK_SPACE)) {
		bowW.translation_.x = worldTransform_.translation_.x;
		bowW.translation_.y = worldTransform_.translation_.y;
		bowW.translation_.z = worldTransform_.translation_.z;

		bowW.rotation_.x = worldTransform_.rotation_.x;
		bowW.rotation_.y = worldTransform_.rotation_.y;
		bowW.rotation_.z = worldTransform_.rotation_.z;

		Gravity = 0.3f;

		flg = true;

		count = 0;

		speed = 1.6f;

		bowD = direction;

		hozon.x = tarW.translation_.x;
		hozon.y = tarW.translation_.y;
		hozon.z = tarW.translation_.z;
	}

	if (flg) {
		bowW.translation_.y += Gravity;
		bowW.translation_.x += speed;
		/*if (hozon.x > bowW.translation_.x)
			bowW.translation_.x += speed;
		else {
			bowW.translation_.x -= speed;
		}

		if (hozon.z > bowW.translation_.z)
			bowW.translation_.z += speed;
		else {
			bowW.translation_.z -= speed
		}*/

		//カウント式
		count++;
		if (count >= 10) {
			//重力を少し下げる	
			Gravity -= 0.1f;
			//カウントリセット
			count = 0;
			//傾き
			bowW.rotation_.z = Gravity + (count / 100);
			//速度を落とす
			speed -= 0.03f;
		}
	}

	//
	worldTransform2_.translation_.x = worldTransform_.translation_.x + (10 * direction.x);
	worldTransform2_.translation_.z = worldTransform_.translation_.z + (10 * direction.z);

	//行列の再計算
	worldTransform_.UpdateMatrix();
	worldTransform2_.UpdateMatrix();
	viewProjection_.UpdateMatrix();
	viewProjection2_.UpdateMatrix();

	bowW.UpdateMatrix();
	bowT.UpdateMatrix();

	tarW.UpdateMatrix();
	tarV.UpdateMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	model_->Draw(worldTransform2_, viewProjection_, textureHandle_);
	model_->Draw(bowW, viewProjection_, textureHandle_);
	//model_->Draw(tarW, viewProjection_, textureHandle_);

	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	debugText_->SetPos(50, 20);
	debugText_->Printf("z:(%f,%f)", hozon.z, bowW.translation_.z);

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
