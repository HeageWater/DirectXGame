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

	/*for (int i = 0; i < size; i++) {
	    int ij = 1;
	    if (i % 2 == 0)
	        ij = -1;
	    worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};
	    worldTransform_[i].translation_ = {-5.0f, 0.0f, 0.0f};
	    worldTransform_[i].Initialize();
	}*/

	worldTransform_[0].scale_ = {3.0f, 3.0f, 3.0f};
	worldTransform_[0].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_[0].translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_[0].Initialize();

	//カメラ支店
	viewProjection_.eye = {0, 0, -50};

	//注視点
	viewProjection_.target = {0, 0, 0};

	//上方向
	viewProjection_.up = {0.0f, 1.0f, 0.0f};

	//ビュープロじぇくション
	viewProjection_.Initialize();
}

void GameScene::Update() {

	viewProjection_.eye.x += sinf(0.2f);
	viewProjection_.eye.z += cosf(0.2f);

	//direction.x = (cos(worldTransform_[0].rotation_.y) - sin(worldTransform_[0].rotation_.z));
	//direction.z = (cos(worldTransform_[0].rotation_.x) - sin(worldTransform_[0].rotation_.y)) - 1;

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

	/*for (int i = 0; i < size; i++) {
	    model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
	}*/

	model_->Draw(worldTransform_[0], viewProjection_, textureHandle_);

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

	/*debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
	  viewProjection_.target.z);

	debugText_->SetPos(50, 110);
	debugText_->Printf(
	  "up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);*/

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
