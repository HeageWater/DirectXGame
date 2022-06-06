#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

#define PI 3.1415

float ReturnRadian(float n) { return n * PI / 180; }

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("Mario.jpg");

	// 3Dモデルの生成
	model_ = Model::Create();

	// X,Y,Z方向のスケ-リングの設定
	worldTransform_.scale_ = {3.0f, 3.0f, 3.0f};

	// value = dist(engine);

	//初期化

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//スケ-リング行列の宣言
	Matrix4 matScale;

	//単位行列を代入
	matScale.Reset();
	//スケ-リング倍率を行列に設定する
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;

	//単位行列を代入
	worldTransform_.matWorld_.Reset();

	// matScaleを掛けて代入
	worldTransform_.matWorld_.m[0][0] *= matScale.m[0][0];
	worldTransform_.matWorld_.m[1][1] *= matScale.m[1][1];
	worldTransform_.matWorld_.m[2][2] *= matScale.m[2][2];


	//x,y,z
	// worldTransform_.rotation_ = { ReturnRadian(45.0f) ,ReturnRadian(45.0f),ReturnRadian(45.0f) };
	
	 // x,y,z
	 worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	//合成用回転行列
	Matrix4 matRot;

	//各軸の回転行列
	Matrix4 matrotX, matrotY, matrotZ;

	//単位行列代入
	matrotZ.Reset();

	matrotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	matrotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	matrotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	matrotZ.m[1][1] = cos(worldTransform_.rotation_.z);

	//単位行列代入
	matrotX.Reset();

	matrotX.m[1][1] = cos(worldTransform_.rotation_.x);
	matrotX.m[1][2] = sin(worldTransform_.rotation_.x);
	matrotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	matrotX.m[2][2] = cos(worldTransform_.rotation_.x);

	//単位行列代入
	matrotY.Reset();

	matrotY.m[0][0] = cos(worldTransform_.rotation_.y);
	matrotY.m[0][2] = -sin(worldTransform_.rotation_.y);
	matrotY.m[2][0] = sin(worldTransform_.rotation_.y);
	matrotY.m[2][2] = cos(worldTransform_.rotation_.y);

	matRot = matrotZ;
	matRot *= matrotX;
	matRot *= matrotY;

	//単位行列代入
	//worldTransform_.matWorld_.Reset();
	worldTransform_.matWorld_ *= matRot;
	worldTransform_.TransferMatrix();

	//平行移動
	worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
	
	//平行移動行列宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;

	//単位行列代入
	//worldTransform_.matWorld_.Reset();
	worldTransform_.matWorld_ *= matTrans;
	worldTransform_.TransferMatrix();

	//行列の転送
	worldTransform_.TransferMatrix();

	viewProjection_.eye = { 0,0,+10 };

	viewProjection_.target = { 0,0,0 };

	// viewProjection_.up = { cosf(PI / 4.0f),sinf(PI / 4.0f),4.0f};

	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向が参照するビューを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビューを指定する(アドレス渡し)
	// PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update()
{
	{
		///デバッグカメラの更新
		////debugCamera_->Update();

		////視点移動ベクトル
		// Vector3 move = { 0,0,0 };

		////視点移動速度
		// const float kEyeSpeed = 0.2f;

		////視点移動速度
		// const float kTargetSpeed = 0.2f;

		////上方向の回転速度[ラジアン/frame]
		// const float kUpRotSpeed = 0.05f;

		// if (input_->PushKey(DIK_SPACE))
		//{
		//	viewAngle += kUpRotSpeed;

		//	viewAngle = fmodf(viewAngle,PI * 2.0f);
		//}

		////押した方向で移動ベクトルを変更
		// if (input_->PushKey(DIK_W))
		//{
		//	move.z += kEyeSpeed;
		// }
		// else if (input_->PushKey(DIK_S))
		//{
		//	move.z -= kEyeSpeed;
		// }

		// if (input_->PushKey(DIK_LEFT))
		//{
		//	move.x -= kTargetSpeed;
		// }
		// else if (input_->PushKey(DIK_RIGHT))
		//{
		//	move.x += kTargetSpeed;
		// }

		////視点移動(ベクトルの加算)
		// viewProjection_.eye += move;

		////視点移動(ベクトルの加算)
		// viewProjection_.target += move;

		// viewProjection_.up = {cosf(viewAngle),sinf(viewAngle),0};

		////行列の再計算
		// viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() 
{
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

	////でバック
	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

	//ライン描画が参照するビューを指定する(アドレス渡し)
	/*PrimitiveDrawer::GetInstance()->DrawLine3d
	(Vector3(0,0,0), Vector3(1280,720,0), Vector4(255, 255, 0, 200));*/
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
