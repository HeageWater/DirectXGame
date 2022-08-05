﻿#include "GameScene.h"
#include "TextureManager.h"
#include "PrimitiveDrawer.h"
#include "MathUtility.h"
#include <cassert>

#define PI = 3.1415;

using namespace DirectX;

void UpdateMatrix(WorldTransform world) {

	Matrix4 matScale, matRot, matTrans;
	
	// スケール、回転、平行移動行列の計算
	matScale = MathUtility::Matrix4Identity();
	matScale.m[0][0] = world.scale_.x;
	matScale.m[1][1] = world.scale_.y;
	matScale.m[2][2] = world.scale_.z;
	matScale.m[3][3] = 1;

	//回転
	matRot = MathUtility::Matrix4Identity();
	matRot = MathUtility::Matrix4RotationX(world.rotation_.z);
	matRot *= MathUtility::Matrix4RotationX(world.rotation_.x);
	matRot *= MathUtility::Matrix4RotationX(world.rotation_.y);
	
	//移動
	matTrans = MathUtility::Matrix4Identity();
	matTrans = MathUtility::Matrix4Translation(
	  world.translation_.x, world.translation_.y, world.translation_.z);

	// ワールド行列の合成
	world.matWorld_ = MathUtility::Matrix4Identity(); // 変形をリセット
	world.matWorld_ *= matScale;          // ワールド行列にスケーリングを反映
	world.matWorld_ *= matRot;            // ワールド行列に回転を反映
	world.matWorld_ *= matTrans;          // ワールド行列に平行移動を反映

	world.TransferMatrix();
}

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete sprite_;
	delete debugcamera;
}

//rayの当たり判定
bool GameScene::dires(Ray ray, Sphere sphere) {

	Vector3 tempV = sphere.position - ray.position;

	Vector3 n;
	//= ray.direction;

	n.x = ray.direction.x;
	n.y = ray.direction.y;
	n.z = ray.direction.z;

	n.normalize();

	float tempL = tempV.dot(n);

	n *= tempL;

	Vector3 tempPos = ray.position + n;

	Vector3 a = sphere.position - tempPos;

	if (tempL < 0) {
		return false;
	}

	if (tempL > ray.direction.length()) {
		return false;
	}

	return a.length() < sphere.r;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("cube//cube.jpg");

	debugcamera = new DebugCamera(1280, 720);
	model_ = Model::Create();
	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	//初期化
	worldTransform_.Initialize();
	worldTransformK_.Initialize();
	worldTransformA_.Initialize();

	viewProjection_.Initialize();

	//移動する物体
	worldTransform_.scale_ = {1.0f, 1.0f, 15.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	//表示される物体
	worldTransformA_.scale_ = {5.0f, 5.0f, 5.0f};
	worldTransformA_.translation_ = {-10.0f, 0.0f, 0.0f};

	//判定取る物体
	worldTransformK_.scale_ = {3.0f, 3.0f, 3.0f};
	worldTransformK_.translation_ = {0.0f, 0.0f, 30.0f};

	ray.direction = {100, 0, 0};

	ray.position.x = worldTransform_.translation_.x;
	ray.position.y = worldTransform_.translation_.y;
	ray.position.z = worldTransform_.translation_.z;

	sphere.position.x = worldTransformK_.translation_.x;
	sphere.position.y = worldTransformK_.translation_.y;
	sphere.position.z = worldTransformK_.translation_.z;

	sphere.r = 3.0f;
}

void GameScene::Update() { 
	debugcamera->Update();

	//正面への移動速度
	const float kTSpeed = 0.3f;

	//バイオ移動
	//横回転
	if (input_->PushKey(DIK_W)) {
		ray.position.y += kTSpeed;
	} else if (input_->PushKey(DIK_S)) {
		ray.position.y -= kTSpeed;
	}

	//正面に進む
	if (input_->PushKey(DIK_D)) {
		ray.position.x += kTSpeed;
	} else if (input_->PushKey(DIK_A)) {
		ray.position.x -= kTSpeed;
	}

	if (input_->PushKey(DIK_E)) {
		ray.position.z += kTSpeed;
	} else if (input_->PushKey(DIK_Q)) {
		ray.position.z -= kTSpeed;
	}

	worldTransform_.translation_.x = ray.position.x;
	worldTransform_.translation_.y = ray.position.y;
	worldTransform_.translation_.z = ray.position.z;

	f = false;

	//レイの当たり判定
	if (dires(ray, sphere)) {
		f = true;
	}

	//行列の再計算
	UpdateMatrix(worldTransform_);
	UpdateMatrix(worldTransformA_);
	UpdateMatrix(worldTransformK_);

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
	
	//動かす物体
	//model_->Draw(worldTransform_, debugcamera->GetViewProjection(), textureHandle_);
	//判定される物体
	model_->Draw(worldTransformK_, debugcamera->GetViewProjection(), textureHandle_);

	//当たっていたら
	if (f) {
		model_->Draw(worldTransformA_, viewProjection_, textureHandle_);
	}


	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugcamera->GetViewProjection());
	Vector4 color = {255, 255, 255, 255};
	Vector3 a = ray.position;
	Vector3 b = ray.position + ray.direction;
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
	
	////でバック
	// debugText_->SetPos(50, 70);
	// debugText_->Printf(
	//   "t r e:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.rotation_.y,
	//   viewProjection_.eye.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf("flag:(%d)", f);

	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "scale:(%f,%f,%f)", worldTransform_.scale_.x, worldTransform_.scale_.y,
	  worldTransform_.scale_.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "rotation:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y,
	  worldTransform_.rotation_.z);

	debugText_->SetPos(50, 130);
	debugText_->Printf(
	  "trans:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
