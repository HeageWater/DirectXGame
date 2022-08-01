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
	textureHandle2_ = TextureManager::Load("reticle.png");


	XMFLOAT4 color = (0.0f,0.0f,0.0f,0.0f);
	sprite_->SetPosition(XMFLOAT2(0,0));
	sprite_->Create(textureHandle2_, sprite_->GetPosition(), color,);

	model_ = Model::Create();

	/*std::random_device seed_gen;

	std::mt19937_64 engine(seed_gen());

	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);

	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);*/

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			worldTransform_[i][j].scale_ = {1.0f, 1.0f, 1.0f};
			worldTransform_[i][j].rotation_ = {0.0f, 0.0f, 0.0f};
			worldTransform_[i][j].translation_ = {-12.0f + (i * 3), -12.0f + (j * 3), 0.0f};

			//ワールドトランスフォーム
			worldTransform_[i][j].Initialize();
		}
	}

	//カメラ支店
	viewProjection_.eye = {0, 0, -50};

	//
	viewProjection_.target = {0, 0, 0};

	//上方向
	viewProjection_.up = {0.0f, 1.0f, 0.0f};

	//カメラ垂直方向視野角
	// viewProjection_.fovAngleY = XMConvertToRadians(45.0f);

	//アスペクト比
	// viewProjection_.aspectRatio = 1.0f;

	//ニアクリップ距離を設定
	// viewProjection_.nearZ = 52.0f;

	//ファークリップ距離を設定
	// viewProjection_.farZ = 53.0f;

	//ビュープロじぇくション
	viewProjection_.Initialize();
}

void GameScene::Update() {

	//視点の移動ベクトル
	XMFLOAT3 move = {0, 0, 0};

	//視点の移動速度
	const float kEyeSpeed = 0.2f;

	const float ZEyeSpeed = 0.1f;

	if (input_->PushKey(DIK_UP)) {
		// viewProjection_.nearZ += 0.1f;
		viewAngle -= ZEyeSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		// viewProjection_.nearZ -= 0.1f;
		viewAngle += ZEyeSpeed;
	}

	//押した方向のベクトル
	if (input_->PushKey(DIK_W)) {
		move = {0, kEyeSpeed, 0};
	} else if (input_->PushKey(DIK_S)) {
		move = {0, -kEyeSpeed, 0};
	}

	if (input_->PushKey(DIK_D)) {
		move = {kEyeSpeed, 0, 0};
	} else if (input_->PushKey(DIK_A)) {
		move = {-kEyeSpeed, 0, 0};
	}

	viewProjection_.fovAngleY = XMConvertToRadians(viewAngle);

	//視点移動
	viewProjection_.target.x += move.x;
	viewProjection_.target.y += move.y;
	viewProjection_.target.z += move.z;

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
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			model_->Draw(worldTransform_[i][j], viewProjection_, textureHandle_);
		}
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
	sprite_->Draw();
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

	debugText_->SetPos(50, 130);
	debugText_->Printf("forAngle(Degree:)%f", XMConvertToDegrees(viewProjection_.fovAngleY));

	debugText_->SetPos(50, 150);
	debugText_->Printf("nearZ:%f", viewProjection_.nearZ);

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
