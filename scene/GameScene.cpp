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

	worldTransform[0].scale_ = {3.0f, 3.0f, 3.0f};
	worldTransform[1].scale_ = {3.0f, 3.0f, 3.0f};
	worldTransform[2].scale_ = {3.0f, 3.0f, 3.0f};

	worldTransform[0].translation_ = {0.0f, 5.0f, 0.0f};
	worldTransform[1].translation_ = {-5.0f, -5.0f, 0.0f};
	worldTransform[2].translation_ = {5.0f, -5.0f, 0.0f};

	//ワールドトランスフォーム
	worldTransform[0].Initialize();
	worldTransform[1].Initialize();
	worldTransform[2].Initialize();

	//カメラ支店
	viewProjection.target = worldTransform[0].translation_;
	//ビュープロじぇくション
	viewProjection.Initialize();
}

void GameScene::Update() {

	////視点の移動ベクトル
	// XMFLOAT3 move = {0, 0, 0};

	//////視点の移動速度
	//// const float kEyeSpeed = 0.2f;

	//////押した方向のベクトル
	//// if (input_->PushKey(DIK_W)) {
	////	move = {0, 0, kEyeSpeed};
	//// } else if (input_->PushKey(DIK_S)) {
	////	move = {0, 0, -kEyeSpeed};
	//// }

	//////視点移動
	//// viewProjection.eye.x += move.x;
	//// viewProjection.eye.y += move.y;
	//// viewProjection.eye.z += move.z;

	////// XMFLOAT3 move = {0, 0, 0};

	//// const float kTargetSpeed = 0.2f;

	//// if (input_->PushKey(DIK_LEFT)) {
	////	move = {-kTargetSpeed, 0, 0};
	//// } else if (input_->PushKey(DIK_RIGHT)) {
	////	move = {kTargetSpeed, 0, 0};
	//// }

	//////視点移動
	//// viewProjection.target.x += move.x;
	//// viewProjection.target.y += move.y;
	//// viewProjection.target.z += move.z;

	////上方向回転速度
	// const float kUpRotSpeed = 0.05f;

	//// if (input_->PushKey(DIK_SPACE)) {
	// viewAngle += kUpRotSpeed;

	//// 2π超えたら0にする
	// viewAngle = fmodf(viewAngle, XM_2PI);
	////}

	// viewProjection.eye.x = cosf(viewAngle);
	// viewProjection.eye.z = sinf(viewAngle);

	////上方向ベクトル計算
	////viewProjection.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};

	if (input_->TriggerKey(DIK_SPACE)) {

		fl++;

		if (fl > 2) {
			fl = 0;
		}
		viewProjection.target = worldTransform[fl].translation_;
	}

	//行列の再計算
	viewProjection.UpdateMatrix();
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

	for (int i = 0; i < 3; i++) {
		model_->Draw(worldTransform[i], viewProjection, textureHandle_);
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
	  "eye:(%f,%f,%f)", viewProjection.eye.x, viewProjection.eye.y, viewProjection.eye.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "target:(%f,%f,%f)", viewProjection.target.x, viewProjection.target.y,
	  viewProjection.target.z);

	debugText_->SetPos(50, 110);
	debugText_->Printf(
	  "up:(%f,%f,%f)", viewProjection.up.x, viewProjection.up.y, viewProjection.up.z);

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
