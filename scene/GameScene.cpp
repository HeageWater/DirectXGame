#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#define PI = 3.1415;

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete sprite_;
}

void GameScene::Initialize() {
	model_ = Model::Create();
	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	textureHandle_ = TextureManager::Load("cube//cube.jpg");

	worldTransform_.scale_ = {3.0f, 3.0f, 3.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	worldTransform2_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform2_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform2_.translation_ = {0.0f, 0.0f, 0.0f};

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	worldTransform2_.Initialize();
	viewProjection2_.Initialize();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//物体の移動ベクトル
	direction = {0, 0, 0};

	for (int i = 0; i < 10; i++) {
		worldTransform[i].Initialize();
		viewProjection[i].Initialize();

		worldTransform[i].scale_ = {3.0f, 3.0f, 3.0f};
		worldTransform[i].rotation_ = {0.0f, 0.0f, 2.0f};
		worldTransform[i].translation_ = {i * 6.0f - 25.0f, 0.0f, 20.0f};

		worldTransform[i].UpdateMatrix();
		viewProjection[i].UpdateMatrix();
	}

	viewProjection_.eye.y = 15.0f;
}

void GameScene::Update() {

	//正面への移動速度
	const float kTSpeed = 0.3f;

	//フラグ切り替え
	if (input_->TriggerKey(DIK_Q)) {
		chengeFlag = !chengeFlag;
	}

	//リセット
	if (input_->TriggerKey(DIK_R)) {
		worldTransform_.scale_ = {3.0f, 3.0f, 3.0f};
		worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
		worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

		direction = {0, 0, 0};

		worldTransform2_.scale_ = {1.0f, 1.0f, 1.0f};
		worldTransform2_.rotation_ = {0.0f, 0.0f, 0.0f};
		worldTransform2_.translation_ = {0.0f, 0.0f, 0.0f};

		viewProjection_.eye.x = 0.0f;
	}

	//バイオ移動
	if (chengeFlag) {
		//横回転
		if (input_->PushKey(DIK_D)) {
			worldTransform_.rotation_.y += 0.05f;
			worldTransform2_.rotation_.y += 0.05f;

			direction.x = (cos(worldTransform_.rotation_.y) - sin(worldTransform_.rotation_.z));
			direction.z = (cos(worldTransform_.rotation_.x) - sin(worldTransform_.rotation_.y)) - 1;

			// 2π超えたら0にする
			worldTransform_.rotation_.y = fmodf(worldTransform_.rotation_.y, XM_2PI);
		} else if (input_->PushKey(DIK_A)) {

			worldTransform_.rotation_.y -= 0.05f;
			worldTransform2_.rotation_.y -= 0.05f;

			direction.x = (cos(worldTransform_.rotation_.y) - sin(worldTransform_.rotation_.z));
			direction.z = (cos(worldTransform_.rotation_.x) - sin(worldTransform_.rotation_.y)) - 1;

			// 2π超えたら0にする
			worldTransform_.rotation_.y = fmodf(worldTransform_.rotation_.y, XM_2PI);
		}

		//正面に進む
		if (input_->PushKey(DIK_W)) {
			worldTransform_.translation_.x += kTSpeed * direction.x;
			worldTransform_.translation_.z += kTSpeed * direction.z;
		} else if (input_->PushKey(DIK_S)) {
			worldTransform_.translation_.x -= kTSpeed * direction.x;
			worldTransform_.translation_.z -= kTSpeed * direction.z;
		}
	}
	//カメラ移動
	else {

		//正面ベクトル
		centerDirection.z = viewProjection_.target.z - viewProjection_.eye.z;
		centerDirection.x = viewProjection_.target.x - viewProjection_.eye.x;
		centerDirection.y = viewProjection_.target.y - viewProjection_.eye.y;

		//長さを取得する
		centerV = sqrtf(
		  centerDirection.x * centerDirection.x + centerDirection.y * centerDirection.y +
		  centerDirection.z * centerDirection.z);

		//正規化
		center.z = centerDirection.z / centerV;
		center.x = centerDirection.x / centerV;
		center.y = centerDirection.y / centerV;

		//仮ベクトルに代入
		rightV = temp.cross(center);

		//右ベクトル正規化
		//rightV = rightV / centerV;

		//正面に進む
		if (input_->PushKey(DIK_W)) {
			worldTransform_.translation_.z += kTSpeed * center.z;
			worldTransform_.translation_.x += kTSpeed * center.x;
		} else if (input_->PushKey(DIK_S)) {
			worldTransform_.translation_.z -= kTSpeed * center.z;
			worldTransform_.translation_.x -= kTSpeed * center.x;
		}

		//左右に移動
		if (input_->PushKey(DIK_A)) {
			worldTransform_.translation_.x -= kTSpeed * rightV.x;
			worldTransform_.translation_.z -= kTSpeed * rightV.z;
		} else if (input_->PushKey(DIK_D)) {
			worldTransform_.translation_.x += kTSpeed * rightV.x;
			worldTransform_.translation_.z += kTSpeed * rightV.z;
		}

		//カメラ移動
		if (input_->PushKey(DIK_LEFT)) {
			viewProjection_.eye.x += kTSpeed;
		} else if (input_->PushKey(DIK_RIGHT)) {
			viewProjection_.eye.x -= kTSpeed;
		}
	}

	worldTransform2_.translation_.x = worldTransform_.translation_.x + (10 * direction.x);
	worldTransform2_.translation_.z = worldTransform_.translation_.z + (10 * direction.z);

	//視点移動
	// viewProjection_.eye.x += move.x;
	// viewProjection_.eye.y += move.y;
	// viewProjection_.eye.z += move.z;

	//行列の再計算
	viewProjection_.UpdateMatrix();
	worldTransform_.UpdateMatrix();

	viewProjection2_.UpdateMatrix();
	worldTransform2_.UpdateMatrix();

	for (int i = 0; i < 10; i++) {
		viewProjection[i].UpdateMatrix();
		worldTransform[i].UpdateMatrix();
	}
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

	for (int i = 0; i < 10; i++) {
		model_->Draw(worldTransform[i], viewProjection_, textureHandle_);
	}

	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画p
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	sprite_->Draw();
	/// </summary>

	////でバック
	// debugText_->SetPos(50, 70);
	// debugText_->Printf(
	//   "t r e:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.rotation_.y,
	//   viewProjection_.eye.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf("flag:(%d)", chengeFlag);

	debugText_->SetPos(50, 20);
	debugText_->Printf("centerX:(%f)", rightV.x);

	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "rotation:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y,
	  worldTransform_.rotation_.z);

	debugText_->SetPos(50, 110);
	debugText_->Printf("direction:(%f,%f,%f)", direction.x, direction.y, direction.z);

	debugText_->SetPos(50, 130);
	debugText_->Printf(
	  "trans:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
