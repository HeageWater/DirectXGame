#pragma once

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

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	Model* model_ = nullptr;

	//スプライト
	Sprite* sprite_ = nullptr;

	WorldTransform worldTransform_;
	WorldTransform worldTransform2_;
	ViewProjection viewProjection_;
	ViewProjection viewProjection2_;

	WorldTransform tarW;
	ViewProjection tarV;

	WorldTransform bowW;
	ViewProjection bowT;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//サウンド
	uint32_t soundDateHandle_ = 0;

	//音声再生
	uint32_t voiceHandle_ = 0;

	uint32_t value_ = 0;

	Vector3 direction;
	Vector3 bowD;
	Vector3 hozon;

	bool chengeFlag = true;

	float viewAngle = 0.0f;

	//正面ベクトル
	Vector3 centerDirection = {0, 0, 0};

	float centerV;

	Vector3 center = {0, 0, 0};

	//右ベクトル
	Vector3 rightV = {0, 0, 0};
	//仮ベクトル
	Vector3 temp = {0, 1, 0};

	bool flg = false;

	float x, z;

	int count = 0;

	//重力
	 float Gravity = 0.2f;

	 float speed = 0.5f;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
