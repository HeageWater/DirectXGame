#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "Syodome.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "player.h"
#include <DirectXMath.h>
#include <cassert>
#include <list>
#include <memory>
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


  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	DebugCamera* debugcamera = nullptr;

	Model* model_ = nullptr;

	//スプライト
	Sprite* sprite_ = nullptr;

	WorldTransform worldTransform_;

	WorldTransform filed;

	ViewProjection viewProjection_;

	ViewProjection Camera1;
	ViewProjection Camera2;
	ViewProjection Camera3;

	ViewProjection HozonCamera;

	//テクスチャハンドル
	uint32_t Cube = 0;
	uint32_t Filed = 0;
	uint32_t P = 0;
	uint32_t pirasuto = 0;

	//サウンド
	uint32_t soundDateHandle_ = 0;

	//音声再生
	uint32_t voiceHandle_ = 0;

	Player* player = nullptr;
	Player* player2 = nullptr;

	Model* modelP = nullptr;

	int a = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
