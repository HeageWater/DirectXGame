#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

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

	viewProjection_.up = {0.0f, 5.0f, 0.0f};

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

	//背景的な
	for (int i = 0; i < 10; i++) {
		worldTransform[i].scale_ = {3.0f, 3.0f, 3.0f};
		worldTransform[i].rotation_ = {0.0f, 0.0f, 0.0f};
		worldTransform[i].translation_ = {i * 6.0f - (28.0f), 0.0f, 20.0f};

		worldTransform[i].Initialize();
		viewProjection[i].Initialize();
	}
}

void GameScene::Update() {
	//視点の移動ベクトル
	// XMFLOAT3 move = {0, 0, 0};

	//視点の移動速度
	const float kRSpeed = 0.02f;

	//正面への移動速度
	const float kTSpeed = 0.2f;

	//横回転
	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y += 0.05f;
		worldTransform2_.rotation_.y += 0.05f;

		direction.x = (cos(worldTransform_.rotation_.y) - sin(worldTransform_.rotation_.z));
		direction.z = (cos(worldTransform_.rotation_.x) - sin(worldTransform_.rotation_.y)) - 1;

		viewProjection_.eye.x += sinf(direction.x);
		viewProjection_.eye.z += cosf(direction.z);

		// 2π超えたら0にする
		worldTransform_.rotation_.y = fmodf(worldTransform_.rotation_.y, XM_2PI);
	} else if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.y -= 0.05f;
		worldTransform2_.rotation_.y -= 0.05f;

		direction.x = (cos(worldTransform_.rotation_.y) - sin(worldTransform_.rotation_.z));
		direction.z = (cos(worldTransform_.rotation_.x) - sin(worldTransform_.rotation_.y)) - 1;

		viewProjection_.eye.x -= sinf(direction.x);
		viewProjection_.eye.z -= cosf(direction.z);

		// 2π超えたら0にする
		worldTransform_.rotation_.y = fmodf(worldTransform_.rotation_.y, XM_2PI);
	}

	//正面に進む
	if (input_->PushKey(DIK_UP)) {
		worldTransform_.translation_.x += kTSpeed * direction.x;
		worldTransform_.translation_.z += kTSpeed * direction.z;
	} else if (input_->PushKey(DIK_DOWN)) {
		worldTransform_.translation_.x -= kTSpeed * direction.x;
		worldTransform_.translation_.z -= kTSpeed * direction.z;
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
	debugText_->Printf(
	  "eye:(%f,%f,%f)", 
		viewProjection_.eye.x, 
		viewProjection_.eye.y,
		viewProjection_.eye.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "rotation:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y,
	  worldTransform_.rotation_.z);

	debugText_->SetPos(50, 110);
	debugText_->Printf("direction:(%f,%f,%f)", direction.x, direction.y, direction.z);

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
