#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
//#include "PrimitiveDrawer"

#define PI = 3.1415;

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete sprite_;
	delete debugcamera;
}

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

	return a.length() < sphere.r;
}

bool dires(Vector3 start, Vector3 end, Vector3 direction, float r) {

	Vector3 a;

	Vector3 tempV = end - start;

	Vector3 tempP;

	// direction
	float tempL = tempV.dot(direction);

	tempP.x = direction.x * tempL;
	tempP.y = direction.y * tempL;
	tempP.z = direction.z * tempL;

	a = end - tempP;

	return r < a.length();
}

void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("cube//cube.jpg");

	debugcamera = new DebugCamera(1280, 720);
	model_ = Model::Create();
	sprite_ = Sprite::Create(textureHandle_, {100, 50});

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

	//初期化
	worldTransform_.Initialize();
	worldTransformK_.Initialize();
	worldTransformA_.Initialize();

	viewProjection_.Initialize();

	ray.direction = {100, 0, 0};

	ray.position.x = worldTransform_.translation_.x;
	ray.position.y = worldTransform_.translation_.y;
	ray.position.z = worldTransform_.translation_.z;

	sphere.position.x = worldTransformK_.translation_.x;
	sphere.position.y = worldTransformK_.translation_.y;
	sphere.position.z = worldTransformK_.translation_.z;

	sphere.r = 3.0f;

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//物体の移動ベクトル
	direction = {0, 0, 0};

	viewProjection_.eye.y = 10.0f;
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
	


	model_->Draw(worldTransform_, debugcamera->GetViewProjection(), textureHandle_);
	//model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	//model_->Draw(worldTransformK_, viewProjection_, textureHandle_);
	model_->Draw(worldTransformK_, debugcamera->GetViewProjection(), textureHandle_);

	if (f) {
		model_->Draw(worldTransformA_, viewProjection_, textureHandle_);
	}

	//model_->Draw(worldTransform_, debugcamera->GetViewProjection(), textureHandle_);
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画p
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	// sprite_->Draw();
	/// </summary>

	////でバック
	// debugText_->SetPos(50, 70);
	// debugText_->Printf(
	//   "t r e:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.rotation_.y,
	//   viewProjection_.eye.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf("flag:(%d)", f);

	/*debugText_->SetPos(50, 20);
	debugText_->Printf("centerX:(%f)", rightV.x);*/

	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "rotation:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y,
	  worldTransform_.rotation_.z);

	debugText_->SetPos(50, 110);
	debugText_->Printf("direction:(%f,%f,%f)", direction.x, direction.y, direction.z);

	debugText_->SetPos(50, 130);
	debugText_->Printf(
	  "trans:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
