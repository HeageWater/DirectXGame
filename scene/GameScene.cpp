#include "GameScene.h"
#include "MathUtility.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

#define PI = 3.1415;

using namespace DirectX;

void GameScene::CheckAllCollisions() {
	Vector3 posA, posB;

	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();

	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();

	//自キャラと敵の弾
#pragma region

	posB = player->GetWorldPosition();

	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		posA = enemy->GetWorldPosition();

		if (
		  (posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y) +
		    (posA.z - posB.z) * (posA.z - posB.z) <=
		  (3 + 1) * (3 + 1)) {
			player->OnCollision();

			bullet->OnCollision();
		}
	}

#pragma endregion

//#pragma region
//#pragma endregion
//
//#pragma region
//#pragma endregion
}

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
	delete enemy;
	delete syodome;
	delete modelSkydome;
}

// rayの当たり判定
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
	textureHandle2_ = TextureManager::Load("mario.jpg");
	textureHandle3_ = TextureManager::Load("skydome//Fine_Basin.jpg");
	textureHandle4_ = TextureManager::Load("white.png");

	modelSkydome = Model::CreateFromOBJ("skydome", true);

	debugcamera = new DebugCamera(1280, 720);
	model_ = Model::Create();
	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	player = new Player();

	player->Initialize(model_, textureHandle_);

	enemy = new Enemy();

	enemy->Initialize(model_, textureHandle2_);

	enemy->SetPlayer(player);

	syodome = new Syodome();

	syodome->Initialize(modelSkydome, textureHandle3_);

	//初期化
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	//移動する物体
	// worldTransform_.scale_ = {1.0f, 1.0f, 15.0f};
	// worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	// worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	// viewProjection_.target = {};
	viewProjection_.up = {0.0f, 0.1f, 0.0f};
	// viewProjection_.eye = {};

	filed.Initialize();

	filed.scale_ = {50.0f, 1.0f, 50.0f};
	filed.rotation_ = {0.0f, 0.0f, 0.0f};
	filed.translation_ = {0.0f, -20.f, 0.0f};

	//敵に自キャラのアドレス渡し
	enemy->SetPlayer(player);

	UpdateMatrix(filed);
}

void GameScene::Update() {

	debugcamera->Update();

	player->Update();

	enemy->Update();

	syodome->Update();

	CheckAllCollisions();

	//行列の再計算
	// UpdateMatrix(worldTransform_);

	/*const float kUpspeed = 0.05f;

	if (input_->PushKey(DIK_SPACE)) {
	    viewAngle += kUpspeed;
	    viewAngle = fmodf(viewAngle, XM_PI);
	}*/

	viewProjection_.eye = {0.0f, 35.0f, -100.0f};

	viewProjection_.target = {0.0f, -25.0f, 0.0f};

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
	//// syodome->Draw(debugcamera->GetViewProjection());
	// player->Draw(debugcamera->GetViewProjection());
	// enemy->Draw(debugcamera->GetViewProjection());
	// model_->Draw(filed, debugcamera->GetViewProjection(), textureHandle4_);

	// syodome->Draw(viewProjection_);
	player->Draw(viewProjection_);
	enemy->Draw(viewProjection_);
	model_->Draw(filed, viewProjection_, textureHandle4_);
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
	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "viewProjection_:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y,
	  viewProjection_.up.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "rotation:(%f,%f,%f)", filed.rotation_.x, filed.rotation_.y, filed.rotation_.z);

	debugText_->SetPos(50, 130);
	debugText_->Printf(
	  "trans:(%f,%f,%f)", filed.translation_.x, filed.translation_.y, filed.translation_.z);
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
