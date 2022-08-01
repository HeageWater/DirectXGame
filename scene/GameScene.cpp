#include "GameScene.h"
#include "TextureManager.h"
#include "PrimitiveDrawer.h"
#include "MathUtility.h"
#include <cassert>


void UpdateMatrix(WorldTransform world) {

	Matrix4 matScale, matRot, matTrans;
	
	// スケール、回転、平行移動行列の計算
	matScale.m[0][0] = world.scale_.x;
	matScale.m[1][1] = world.scale_.y;
	matScale.m[2][2] = world.scale_.z;
	matScale.m[3][3] = 1;

	world.matWorld_ = MathUtility::Matrix4Identity();

	matRot = MathUtility::Matrix4Identity();
	matRot = MathUtility::Matrix4RotationX(world.rotation_.z);
	matRot *= MathUtility::Matrix4RotationX(world.rotation_.x);
	matRot *= MathUtility::Matrix4RotationX(world.rotation_.y);
	
	matTrans = MathUtility::Matrix4Translation(
	  world.translation_.x, world.translation_.y, world.translation_.z);

	// ワールド行列の合成
	world.matWorld_ = MathUtility::Matrix4Identity(); // 変形をリセット
	world.matWorld_ *= matScale;          // ワールド行列にスケーリングを反映
	world.matWorld_ *= matRot;            // ワールド行列に回転を反映
	world.matWorld_ *= matTrans;          // ワールド行列に平行移動を反映

	//// 親行列の指定がある場合は、掛け算する
	//if (parent_) {
	//	matWorld_ *= parent_->matWorld_;
	//}

	//// 定数バッファに書き込み
	//constMap->matWorld = matWorld_;
}

GameScene::GameScene() {}

GameScene::~GameScene() { delete debugcamera; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	debugcamera = new DebugCamera(1280,720);

	worldTransform.Initialize();
	viewProjection.Initialize();

	worldTransform.scale_ = {1.0f,1.0f,1.0f};
	worldTransform.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform.rotation_ = {0.0f, 0.0f, 0.0f};

	viewProjection.target = {0, 0, 0};
	viewProjection.up = {0, 0, 0};
	viewProjection.eye = {0, 0, 0};
}

void GameScene::Update() { 
	debugcamera->Update();

	UpdateMatrix(worldTransform);
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
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugcamera->GetViewProjection());

	Vector4 color = {255, 255, 255, 0};

	Vector3 a = {0, 0, 0};
	Vector3 b = {100, 100, 100};

	PrimitiveDrawer::GetInstance()->DrawLine3d(a, b, color);
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

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
