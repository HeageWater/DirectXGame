﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete sprite_;
}

void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();
	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	soundDateHandle_ = audio_->LoadWave("se_sad03.wav");

	audio_->PlayWave(soundDateHandle_);

	voiceHandle_ = audio_->PlayWave(soundDateHandle_, true);
}

void GameScene::Update() {
	XMFLOAT2 position = sprite_->GetPosition();

	position.x += 2.0f;
	position.y += 1.0f;

	sprite_->SetPosition(position);

	if (input_->TriggerKey(DIK_SPACE)) {
		audio_->StopWave(voiceHandle_);
	}

	// valueの値を加算
	value_++;

	//文字列と値を代入
	std::string strDebug = std::string("Value:") + std::to_string(value_);
	debugText_->Print(strDebug, 50, 50, 1.0f);
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
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
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

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
