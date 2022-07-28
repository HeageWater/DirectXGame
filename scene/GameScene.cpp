#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");

	model_ = Model::Create();

	worldTransform_[0].Initialize();

	//頭
	worldTransform_[1].translation_ = {0, 4.5f, 0};
	worldTransform_[1].parent_ = &worldTransform_[0];
	worldTransform_[1].Initialize();

	//右腕
	worldTransform_[2].translation_ = {4.5f, 0, 0};
	worldTransform_[2].parent_ = &worldTransform_[0];
	worldTransform_[2].Initialize();

	//左腕
	worldTransform_[3].translation_ = {-4.5f, 0, 0};
	worldTransform_[3].parent_ = &worldTransform_[0];
	worldTransform_[3].Initialize();

	//右足
	worldTransform_[4].translation_ = {4.5f, -4.5f, 0};
	worldTransform_[4].parent_ = &worldTransform_[6];
	worldTransform_[4].Initialize();

	//左足
	worldTransform_[5].translation_ = {-4.5f, -4.5f, 0};
	worldTransform_[5].parent_ = &worldTransform_[6];
	worldTransform_[5].Initialize();

	//下半身
	worldTransform_[6].translation_ = {0, -4.5f, 0};
	worldTransform_[6].Initialize();

	//カメラ支店
	viewProjection_.eye = {0, 0, -50};

	viewProjection_.target = {0, 0, 0};

	//上方向
	viewProjection_.up = {0.0f, 1.0f, 0.0f};

	//ビュープロじぇくション
	viewProjection_.Initialize();
}

void GameScene::Update() {

	//視点の移動ベクトル
	XMFLOAT3 move = {0, 0, 0};

	//視点の移動速度
	const float kCharaSpeed = 0.2f;

	if (input_->PushKey(DIK_RIGHT)) {
		move = {kCharaSpeed, 0, 0};
	} else if (input_->PushKey(DIK_LEFT)) {
		move = {-kCharaSpeed, 0, 0};
	}

	//回転速度
	const float RCharaSpeed = 0.02f;

	if (input_->PushKey(DIK_I)) {
		worldTransform_[0].rotation_.y += RCharaSpeed;
	} else if (input_->PushKey(DIK_U)) {
		worldTransform_[0].rotation_.y -= RCharaSpeed;
	}

	if (input_->PushKey(DIK_J)) {
		worldTransform_[6].rotation_.y += RCharaSpeed;
	} else if (input_->PushKey(DIK_K)) {
		worldTransform_[6].rotation_.y -= RCharaSpeed;
	}

	//注視点移動
	worldTransform_[0].translation_.x += move.x;
	worldTransform_[0].translation_.y += move.y;
	worldTransform_[0].translation_.z += move.z;

	worldTransform_[6].translation_.x += move.x;
	worldTransform_[6].translation_.y += move.y;
	worldTransform_[6].translation_.z += move.z;

	for (int i = 0; i < 7; i++) {
		worldTransform_[i].UpdateMatrix();
	}

	//行列の再計算
	viewProjection_.UpdateMatrix();
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

	for (int i = 0; i < 7; i++) {
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
	}
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

	//でバック
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
	  viewProjection_.target.z);

	debugText_->SetPos(50, 110);
	debugText_->Printf(
	  "up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
