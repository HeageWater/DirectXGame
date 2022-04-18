#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete sprite_;
}

void GameScene::Initialize() 
{
	model_ = Model::Create();
	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};
	worldTransform_.rotation_ = {0.0f, XM_PI / 4.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 15.0f};

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
}

void GameScene::Update() 
{
	//視点の移動ベクトル
	XMFLOAT3 move = {0, 0, 0};

	//視点の移動速度
	const float kEyeSpeed = 0.02f;

	if (input_->PushKey(DIK_LEFT))
	{
		//a += kEyeSpeed;
	} 
	else if (input_->PushKey(DIK_RIGHT))
	{
		//a -= kEyeSpeed;
	}

	if (input_->PushKey(DIK_UP))
	{
		// a += kEyeSpeed;
	}
	else if (input_->PushKey(DIK_DOWN)) 
	{
		// a -= kEyeSpeed;
	}

	//視点移動
	viewProjection_.eye.x += move.x;
	viewProjection_.eye.y += move.y;
	viewProjection_.eye.z += move.z;

	//worldTransform_.rotation_ = {0, a, 0};

	//行列の再計算
	viewProjection_.UpdateMatrix();
	worldTransform_.UpdateMatrix();
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
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	sprite_->Draw();
	/// </summary>

	//でバック
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
