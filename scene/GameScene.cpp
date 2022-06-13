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

//ワールドアップデート
void WorldTransformUpdate(WorldTransform player) {
	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(player.scale_.x, player.scale_.y, player.scale_.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(player.rotation_.z);
	matRot *= XMMatrixRotationX(player.rotation_.x);
	matRot *= XMMatrixRotationY(player.rotation_.y);
	matTrans =
	  XMMatrixTranslation(player.translation_.x, player.translation_.y, player.translation_.z);

	// ワールド行列の合成
	player.matWorld_ = XMMatrixIdentity(); // 変形をリセット
	player.matWorld_ *= matScale;   // ワールド行列にスケーリングを反映
	player.matWorld_ *= matRot;     // ワールド行列に回転を反映
	player.matWorld_ *= matTrans;    // ワールド行列に平行移動を反映

	// 親行列の指定がある場合は、掛け算する
	if (player.parent_) {
		player.matWorld_ *= player.parent_->matWorld_;
	}

	// 定数バッファに書き込み
	player.constMap->matWorld = player.matWorld_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("cube//cube.jpg");

	// 3Dモデルの生成
	model_ = Model::Create();

	for (int i = 0; i < 10; i++) {

		worldTransform[i].scale_ = {3.0f, 3.0f, 3.0f};
		worldTransform[i].rotation_ = {0.0f, 0.0f, 0.0f};
		worldTransform[i].translation_ = {i * 3.0f, 0.0f, 0.0f};

		worldTransform[i].Initialize();
		viewProjection[i].Initialize();
	}

	// X,Y,Z方向のスケ-リングの設定
	player_WorldTransform_.scale_ = {3.0f, 3.0f, 3.0f};
	player_WorldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	player_WorldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	//初期化
	player_WorldTransform_.Initialize();
	player_ViewProjection_.Initialize();

	// value = dist(engine);

	//手動
	{
		////ワールドトランスフォームの初期化
		// player_WorldTransform_.Initialize();

		////スケ-リング行列の宣言
		// Matrix4 matScale;

		////単位行列を代入
		// matScale.Reset();
		////スケ-リング倍率を行列に設定する
		// matScale.m[0][0] = player_WorldTransform_.scale_.x;
		// matScale.m[1][1] = player_WorldTransform_.scale_.y;
		// matScale.m[2][2] = player_WorldTransform_.scale_.z;

		////単位行列を代入
		// player_WorldTransform_.matWorld_.Reset();

		//// matScaleを掛けて代入
		// player_WorldTransform_.matWorld_.m[0][0] *= matScale.m[0][0];
		// player_WorldTransform_.matWorld_.m[1][1] *= matScale.m[1][1];
		// player_WorldTransform_.matWorld_.m[2][2] *= matScale.m[2][2];

		//// x,y,z
		////  worldTransform_.rotation_ = { ReturnRadian(45.0f)
		///,ReturnRadian(45.0f),ReturnRadian(45.0f)};

		//// x,y,z
		// player_WorldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

		////合成用回転行列
		// Matrix4 matRot;

		////各軸の回転行列
		// Matrix4 matrotX, matrotY, matrotZ;

		////単位行列代入
		// matrotZ.Reset();

		// matrotZ.m[0][0] = cos(player_WorldTransform_.rotation_.z);
		// matrotZ.m[0][1] = sin(player_WorldTransform_.rotation_.z);
		// matrotZ.m[1][0] = -sin(player_WorldTransform_.rotation_.z);
		// matrotZ.m[1][1] = cos(player_WorldTransform_.rotation_.z);

		////単位行列代入
		// matrotX.Reset();

		// matrotX.m[1][1] = cos(player_WorldTransform_.rotation_.x);
		// matrotX.m[1][2] = sin(player_WorldTransform_.rotation_.x);
		// matrotX.m[2][1] = -sin(player_WorldTransform_.rotation_.x);
		// matrotX.m[2][2] = cos(player_WorldTransform_.rotation_.x);

		////単位行列代入
		// matrotY.Reset();

		// matrotY.m[0][0] = cos(player_WorldTransform_.rotation_.y);
		// matrotY.m[0][2] = -sin(player_WorldTransform_.rotation_.y);
		// matrotY.m[2][0] = sin(player_WorldTransform_.rotation_.y);
		// matrotY.m[2][2] = cos(player_WorldTransform_.rotation_.y);

		// matRot = matrotZ;
		// matRot *= matrotX;
		// matRot *= matrotY;

		////単位行列代入
		//// worldTransform_.matWorld_.Reset();
		// player_WorldTransform_.matWorld_ *= matRot;
		// player_WorldTransform_.TransferMatrix();

		////平行移動
		// player_WorldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

		////平行移動行列宣言
		// Matrix4 matTrans = MathUtility::Matrix4Identity();

		// matTrans.m[3][0] = player_WorldTransform_.translation_.x;
		// matTrans.m[3][1] = player_WorldTransform_.translation_.y;
		// matTrans.m[3][2] = player_WorldTransform_.translation_.z;

		////単位行列代入
		//// worldTransform_.matWorld_.Reset();
		// player_WorldTransform_.matWorld_ *= matTrans;
		// player_WorldTransform_.TransferMatrix();

		////行列の転送
		// player_WorldTransform_.TransferMatrix();

		// player_ViewProjection_.eye = {0, 0, +10};

		// player_ViewProjection_.target = {0, 0, 0};

		//// viewProjection_.up = { cosf(PI / 4.0f),sinf(PI / 4.0f),4.0f};

		// player_ViewProjection_.Initialize();
	}

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向が参照するビューを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&player_ViewProjection_);

	//ライン描画が参照するビューを指定する(アドレス渡し)
	// PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {
	{
		//デバッグカメラの更新
		// debugCamera_->Update();

		//視点移動ベクトル
		//Vector3 move = {0, 0, 0};

		//視点移動速度
		const float kTSpeed = 0.2f;

		//上方向の回転速度[ラジアン/frame]
		const float kUpRotSpeed = 0.05f;

		//回転
		if (input_->PushKey(DIK_SPACE)) {
			viewAngle += kUpRotSpeed;

			viewAngle = fmodf(viewAngle, PI * 2.0f);
		}

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			player_WorldTransform_.rotation_.y += 0.05f;

			player_direction.x =
			  (cos(player_WorldTransform_.rotation_.y) - sin(player_WorldTransform_.rotation_.z));
			player_direction.z =
			  (cos(player_WorldTransform_.rotation_.x) - sin(player_WorldTransform_.rotation_.y)) -
			  1;

			// 2π超えたら0にする
			player_WorldTransform_.rotation_.y = fmodf(player_WorldTransform_.rotation_.y, XM_2PI);
		} else if (input_->PushKey(DIK_RIGHT)) {
			player_WorldTransform_.rotation_.y -= 0.05f;

			player_direction.x =
			  (cos(player_WorldTransform_.rotation_.y) - sin(player_WorldTransform_.rotation_.z));
			player_direction.z =
			  (cos(player_WorldTransform_.rotation_.x) - sin(player_WorldTransform_.rotation_.y)) -
			  1;

			// 2π超えたら0にする
			player_WorldTransform_.rotation_.y = fmodf(player_WorldTransform_.rotation_.y, XM_2PI);
		}

		//正面に進む
		if (input_->PushKey(DIK_UP)) {
			player_WorldTransform_.translation_.x += kTSpeed * player_direction.x;
			player_WorldTransform_.translation_.z += kTSpeed * player_direction.z;
		} else if (input_->PushKey(DIK_DOWN)) {
			player_WorldTransform_.translation_.x -= kTSpeed * player_direction.x;
			player_WorldTransform_.translation_.z -= kTSpeed * player_direction.z;
		}

		//リセット
		if (input_->PushKey(DIK_R)) {
		  /*player_ViewProjection_.eye.x = 0;
			player_ViewProjection_.eye.y = 0;
			player_ViewProjection_.eye.z = 10;

			player_ViewProjection_.target.x = 0;
			player_ViewProjection_.target.y = 0;
			player_ViewProjection_.target.z = 0;*/
		}

		//視点移動(ベクトルの加算)
		//player_ViewProjection_.eye += move;

		//視点移動(ベクトルの加算)
		//player_ViewProjection_.target += move;

		//上方向ベクトル(右上45度)
		// player_ViewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0};

		//行列の再計算
		player_ViewProjection_.UpdateMatrix();
		WorldTransformUpdate(player_WorldTransform_);
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
	model_->Draw(player_WorldTransform_, player_ViewProjection_, textureHandle_);

	for (int i = 0; i < 10; i++) {
		model_->Draw(worldTransform[i], viewProjection[i], textureHandle_);
	}
	////でバック
	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "translation:(%f,%f,%f)", player_WorldTransform_.translation_.x,
	  player_WorldTransform_.translation_.y, player_WorldTransform_.translation_.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "rotation:(%f,%f,%f)", player_WorldTransform_.rotation_.x, player_WorldTransform_.rotation_.y,
	  player_WorldTransform_.rotation_.z);

	debugText_->SetPos(50, 110);
	debugText_->Printf(
	  "direction:(%f,%f,%f)", player_direction.x, player_direction.y, player_direction.z);

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