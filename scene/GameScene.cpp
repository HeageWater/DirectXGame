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

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//物体の移動ベクトル
	direction = {0, 0, 0};

	worldTransform[0].translation_ = {4.5f, 0.0f, 0.0f};
	worldTransform[0].Initialize();

	for (int i = 1; i < size; i++) {
		worldTransform[i].translation_ = {0, 0, 4.5f};
		worldTransform[i].parent_ = &worldTransform[i - 1];
		worldTransform[i].Initialize();
	}
}

void GameScene::Update() {
	//視点の移動ベクトル
	XMFLOAT3 move = {0, 0, 0};

	//視点の移動速度
	const float kRSpeed = 0.2f;

	const float RoteSpeed = 0.05f;

	//正面への移動速度
	const float kTSpeed = 0.2f;

	//横回転
	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y += RoteSpeed;

		direction.x = (cos(worldTransform_.rotation_.y) - sin(worldTransform_.rotation_.y));
		direction.z = (cos(worldTransform_.rotation_.y) - sin(worldTransform_.rotation_.y));

		// 2π超えたら0にする
		worldTransform_.rotation_.y = fmodf(worldTransform_.rotation_.y, XM_2PI);

		// viewProjection_.eye.x += RoteSpeed * direction.x;
		// viewProjection_.eye.z += RoteSpeed * direction.z;

	} else if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.y -= RoteSpeed;

		viewProjection_.eye.x -= RoteSpeed;

		direction.x = (cos(worldTransform_.rotation_.y) - sin(worldTransform_.rotation_.z));
		direction.z = (cos(worldTransform_.rotation_.x) - sin(worldTransform_.rotation_.y)) - 1;

		// 2π超えたら0にする
		worldTransform_.rotation_.y = fmodf(worldTransform_.rotation_.y, XM_2PI);
	}

	//正面に進む

	//視点移動
	XMFLOAT3 result(0, 0, 0);
	XMFLOAT3 front(0, 0, 1);

	// y軸回りの回転行列演算
	result.x =
	  cos(worldTransform_.rotation_.y) * front.x + sin(worldTransform_.rotation_.y) * front.z;
	result.z =
	  -sin(worldTransform_.rotation_.y) * front.x + cos(worldTransform_.rotation_.y) * front.z;

	result.x =
	  cos(worldTransform_.rotation_.y) * front.x + sin(worldTransform_.rotation_.y) * front.z;
	result.z =
	  -sin(worldTransform_.rotation_.y) * front.x + cos(worldTransform_.rotation_.y) * front.z;

	front.x = result.x;
	front.z = result.z;

	float moveSpeed = 0.1f;
	if (input_->PushKey(DIK_UP)) {
		worldTransform_.translation_.x += moveSpeed * front.x;
		worldTransform_.translation_.z += moveSpeed * front.z;
	}
	if (input_->PushKey(DIK_DOWN)) {
		worldTransform_.translation_.x -= moveSpeed * front.x;
		worldTransform_.translation_.z -= moveSpeed * front.z;
	}

	worldTransform_.UpdateMatrix();

	XMFLOAT3 camera(0, 0.5f, 0);
	result.x = cos(XMConvertToRadians(180)) * front.x + sin(XMConvertToRadians(180)) * front.z;
	result.z = -sin(XMConvertToRadians(180)) * front.x + cos(XMConvertToRadians(180)) * front.z;

	camera.x = result.x;
	camera.z = result.z;

	float cameraDistance = 30;
	viewProjection_.eye.x = worldTransform_.translation_.x + cameraDistance * camera.x;
	viewProjection_.eye.y = worldTransform_.translation_.y + cameraDistance * camera.y;
	viewProjection_.eye.z = worldTransform_.translation_.z + cameraDistance * camera.z;

	viewProjection_.target.x = worldTransform_.translation_.x;
	viewProjection_.target.y = worldTransform_.translation_.y;
	viewProjection_.target.z = worldTransform_.translation_.z;

	//行列の再計算
	viewProjection_.UpdateMatrix();
	worldTransform_.UpdateMatrix();

	for (int i = 0; i < size; i++) {
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

	for (int i = 0; i < size; i++) {
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
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "Root:(%f,%f,%f)", worldTransform[PartId::Root].translation_.x,
	  worldTransform[PartId::Root].translation_.y, worldTransform[PartId::Root].translation_.z);

	debugText_->SetPos(50, 200);
	debugText_->Printf(
	  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
	  viewProjection_.target.z);

	debugText_->SetPos(50, 250);
	debugText_->Printf(
	  "up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
