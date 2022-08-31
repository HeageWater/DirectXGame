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

	posA = player->GetWorldPosition();

	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		posB = enemy->GetBulletWorldPosition();

		Vector3 anser;

		float R1 = player->playerW.scale_.x;
		float R2 = 1;

		anser.x = (posB.x - posA.x) * (posB.x - posA.x);
		anser.y = (posB.y - posA.y) * (posB.y - posA.y);
		anser.z = (posB.z - posA.z) * (posB.z - posA.z);

		if (anser.x + anser.y + anser.z <= (R1 + R2) * (R1 + R2)) {

			player->OnCollision();

			bullet->OnCollision();
		}
	}

#pragma endregion

	//自キャラと敵キャラ
#pragma region
	posA = player->GetWorldPosition();

	posB = enemy->GetWorldPosition();
	Vector3 anser;

	float R1 = player->playerW.scale_.x;
	float R2 = enemy->EnemyW.scale_.x;

	anser.x = (posB.x - posA.x) * (posB.x - posA.x);
	anser.y = (posB.y - posA.y) * (posB.y - posA.y);
	anser.z = (posB.z - posA.z) * (posB.z - posA.z);

	if (anser.x + anser.y + anser.z <= (R1 + R2) * (R1 + R2)) {

		player->OnCollision();

		enemy->OnCollision();
	}

#pragma endregion

	//敵キャラと自弾
#pragma region
	posB = enemy->GetWorldPosition();
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		posA = player->GetBulletWorldPosition();

		R1 = 1;
		R2 = enemy->EnemyW.scale_.x;

		anser.x = (posB.x - posA.x) * (posB.x - posA.x);
		anser.y = (posB.y - posA.y) * (posB.y - posA.y);
		anser.z = (posB.z - posA.z) * (posB.z - posA.z);

		if (anser.x + anser.y + anser.z <= (R1 + R2) * (R1 + R2)) {

			bullet->OnCollision();

			enemy->OnCollision();
		}
	}
#pragma endregion
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
	delete modelP;
	delete Kyu;
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

	Cube = TextureManager::Load("cube//cube.jpg");
	Mario = TextureManager::Load("mario.jpg");
	Skydome = TextureManager::Load("skydome//Fine_Basin.jpg");
	Filed = TextureManager::Load("white.png");
	titleEA = TextureManager::Load("T.jpg");
	StartB = TextureManager::Load("Start.png");
	P = TextureManager::Load("Player//Player.png");
	shot = TextureManager::Load("kyu//kyu.png");

	modelP = Model::CreateFromOBJ("Player", true);
	modelSkydome = Model::CreateFromOBJ("skydome", true);
	Kyu = Model::CreateFromOBJ("kyu", true);

	intoro = audio_->LoadWave("SE//intoro.wav");
	BGM = audio_->LoadWave("SE//ryoutou.wav");

	debugcamera = new DebugCamera(1280, 720);
	model_ = Model::Create();
	sprite_ = Sprite::Create(Cube, {100, 50});

	player = new Player();

	player->Initialize(modelP, Kyu, P);

	enemy = new Enemy();

	enemy->Initialize(model_, Mario);

	enemy->SetPlayer(player);

	syodome = new Syodome();

	syodome->Initialize(modelSkydome, Skydome);

	//初期化
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	//タイトル画面
	titleEW.Initialize();
	titleEV.Initialize();

	titleEW.scale_ = {128, 72, 0};
	UpdateMatrix(titleEW);

	StartW.Initialize();

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

	titleEW.scale_ = {15, 10, 0};
	titleEW.translation_ = {0, 0, 0};
	UpdateMatrix(titleEW);

	StartW.scale_ = {10, 5, 0};
	StartW.translation_ = {0, -5, 0};
	UpdateMatrix(StartW);

	switch (NowPhase) {
	case Start:

		if (input_->TriggerKey(DIK_SPACE)) {
			NowPhase = Play;
			// BGM
			audio_->PlayWave(intoro, false, 0.05);
		}

		break;
	case Play:

		if (x < 740) {
			x += 1;
		} else if (y < 10) {
			y += 0.1f;
		} else {
			PlayFlg = true;
		}

		if (input_->TriggerKey(DIK_RIGHT)) {
			x++;
		}
		if (input_->TriggerKey(DIK_LEFT)) {
			y--;
		}

		if (PlayFlg) {

			// BGM
			if (audio_->IsPlaying(BGM) != true) {

				audio_->PlayWave(BGM, false, 0.05);
			}

			/*x += 0.01;
			if (x > 935) {
			    x = 740;
			}*/

			debugcamera->Update();

			player->Update();

			enemy->Update(player->playerW);

			syodome->Update();

			CheckAllCollisions();

			if (input_->TriggerKey(DIK_P)) {
				NowPhase = Menu;
				checkbutton = 0;
			}
		}

		break;
	case Menu:
		if (input_->TriggerKey(DIK_P)) {
			NowPhase = Play;
		}
		break;
	default:
		break;
	}

	//行列の再計算
	// UpdateMatrix(worldTransform_);

	/*const float kUpspeed = 0.05f;

	if (input_->PushKey(DIK_SPACE)) {
	    viewAngle += kUpspeed;
	    viewAngle = fmodf(viewAngle, XM_PI);
	}*/

	viewProjection_.up = {0.0f, 0.1f, 0.0f};

	viewProjection_.eye = {
	  cosf((x / 10) / XM_PI) * (x / 10), 80.0f, sinf((x / 10) / XM_PI) * (x / 10) - y};

	viewProjection_.target = {cosf((x / 10) / XM_PI), -25.0f, sinf((x / 10) / XM_PI)};

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
	switch (NowPhase) {
	case Start:
		model_->Draw(StartW, titleEV, StartB);
		model_->Draw(titleEW, titleEV, titleEA);
		// model_->Draw(titleEW, debugcamera->GetViewProjection(), titleE);
		break;
	case Play:
		syodome->Draw(debugcamera->GetViewProjection());
		player->Draw(debugcamera->GetViewProjection());
		enemy->Draw(debugcamera->GetViewProjection());
		model_->Draw(filed, debugcamera->GetViewProjection(), Cube);

		/*syodome->Draw(viewProjection_);
		model_->Draw(filed, viewProjection_, Cube);
		if (PlayFlg) {
			player->Draw(viewProjection_);
			enemy->Draw(viewProjection_);
		}*/
		break;
	case Menu:
		syodome->Draw(viewProjection_);
		player->Draw(viewProjection_);
		enemy->Draw(viewProjection_);
		model_->Draw(filed, viewProjection_, Cube);
		break;
	default:
		break;
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

	//でバック
	debugText_->SetPos(50, 50);
	debugText_->Printf("viewProjection_:(%f,%f,%f)", x, viewProjection_.up.y, viewProjection_.up.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "rotation:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 130);
	debugText_->Printf(
	  "trans:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
	  viewProjection_.target.z);
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
