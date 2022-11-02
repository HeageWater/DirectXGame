#include "GameScene.h"
#include "MathUtility.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
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
	matRot = MathUtility::Matrix4RotationZ(world.rotation_.z);
	matRot *= MathUtility::Matrix4RotationX(world.rotation_.x);
	matRot *= MathUtility::Matrix4RotationY(world.rotation_.y);

	//移動
	matTrans = MathUtility::Matrix4Identity();
	matTrans = MathUtility::Matrix4Translation(
	  world.translation_.x, world.translation_.y, world.translation_.z);

	// ワールド行列の合成
	world.matWorld_ = MathUtility::Matrix4Identity(); // 変形をリセット
	world.matWorld_ *= matScale; // ワールド行列にスケーリングを反映
	world.matWorld_ *= matRot;   // ワールド行列に回転を反映
	world.matWorld_ *= matTrans; // ワールド行列に平行移動を反映

	world.TransferMatrix();
}

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete sprite_;
	delete debugcamera;
	delete player;
	delete modelP;
}

void GameScene::Initialize() {

	//宣言
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	debugcamera = new DebugCamera(1280, 720);
	model_ = Model::Create();
	sprite_ = Sprite::Create(Cube, {100, 50});

	//イラスト読み込み
	Cube = TextureManager::Load("Filed.png");
	P = TextureManager::Load("Player//Player.png");
	pirasuto = TextureManager::Load("playerL.png");

	//モデル読み込み
	modelP = Model::CreateFromOBJ("Player", true);

	//プレイヤー初期化
	player = new Player();
	player->Initialize(modelP,P);

	//初期化
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	//フィールド初期化
	filed.Initialize();

	filed.scale_ = {50.0f, 1.0f, 50.0f};
	filed.rotation_ = {0.0f, 0.0f, 0.0f};
	filed.translation_ = {0.0f, -20.f, 0.0f};

	//ビュー初期化
	viewProjection_.up = {0.0f, 0.1f, 0.0f};

	viewProjection_.eye = {0, 0, 0};

	viewProjection_.target = {0, 0, 0};


	viewProjection_.up = {0.0f, 1.0f, 0.0f};

	viewProjection_.eye = {0.0f, 90.0f,0.0f};

	viewProjection_.target = {0.0f, 0.0f, sinf((64) / XM_PI)};
}

void GameScene::Update() {

	//更新
	UpdateMatrix(filed);
	player->Update(viewProjection_);

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
	model_->Draw(filed, viewProjection_, Cube);
	player->Draw(viewProjection_);
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// ここに前景スプライトの描画処理を追加できる

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "rotation:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
