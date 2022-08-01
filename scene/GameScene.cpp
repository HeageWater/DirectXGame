#include "GameScene.h"
#include "TextureManager.h"
#include "PrimitiveDrawer.h"
#include <cassert>

void UpdateMatrix(WorldTransform world) {

	//Matrix4 matScale, matRot, matTrans;

	////スケールなどの計算
	//matScale = XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z);
	//matRot = XMMatrixIdentity();
	//matRot *= XMMatrixRotationZ(this->rotation.z);
	//matRot *= XMMatrixRotationX(this->rotation.x);
	//matRot *= XMMatrixRotationY(this->rotation.y);

	//matTrans = XMMatrixTranslation(this->position.x, this->position.y, this->position.z);

	//this->world = XMMatrixIdentity();
	//this->world *= matScale;
	//this->world *= matRot;
	//this->world *= matTrans;

	////データ転送
	//this->constMapTransform->mat = this->matWorld * matView * matProjection;


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

	//worldTransform.UpdateMatrix();
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
