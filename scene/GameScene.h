﻿#pragma once

#include "Audio.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Vector3.h"
#include <DirectXMath.h>
using namespace DirectX; 

typedef struct Ray {
	Vector3 position;
	Vector3 direction;
};

typedef struct Sphere {
	Vector3 position;
	float r;
};

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	bool dires(Ray ray, Sphere sphere);

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	Model* model_ = nullptr;

	//スプライト
	Sprite* sprite_ = nullptr;

	WorldTransform worldTransform_;

	WorldTransform worldTransformK_;

	WorldTransform worldTransformA_;

	ViewProjection viewProjection_;
	
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//サウンド
	uint32_t soundDateHandle_ = 0;

	//音声再生
	uint32_t voiceHandle_ = 0;

	uint32_t value_ = 0;

	Vector3 direction;

	bool chengeFlag = true;

	//float viewAngle = 0.0f;

	////正面ベクトル
	//Vector3 centerDirection = {0, 0, 0};

	//float centerV;

	//Vector3 center = {0, 0, 0};

	////右ベクトル
	//Vector3 rightV = {0, 0, 0};
	////仮ベクトル
	//Vector3 temp = {0, 1, 0};

	bool f = false;

	Ray ray;
	Sphere sphere;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
