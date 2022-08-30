#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Vector3.h"
#include <DirectXMath.h>
#include "player.h"
#include "Enemy.h"
#include "Syodome.h"
#include <cassert>
#include <memory>
#include <list>
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

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	bool dires(Ray ray, Sphere sphere);

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	DebugCamera* debugcamera = nullptr;

	//WorldTransform worldTransform;
	//ViewProjection viewProjection;
	Model* model_ = nullptr;

	//スプライト
	Sprite* sprite_ = nullptr;

	WorldTransform worldTransform_;

	WorldTransform filed;

	ViewProjection viewProjection_;

	WorldTransform titleEW;

	ViewProjection titleEV;

	//テクスチャハンドル
	uint32_t Cube = 0;
	uint32_t Mario = 0;
	uint32_t Skydome = 0;
	uint32_t Filed = 0;
	uint32_t titleE = 0;

	//サウンド
	uint32_t soundDateHandle_ = 0;

	//音声再生
	uint32_t voiceHandle_ = 0;

	uint32_t value_ = 0;

	Vector3 direction;

	//std::list<std::unique_ptr<Player>> player;
	//std::list<std::unique_ptr<Enemy>> enemy;

	Player* player = nullptr;
	Enemy* enemy = nullptr;
	Syodome* syodome = nullptr;

	Model* modelSkydome = nullptr;

	float viewAngle = 0.0f;

	float x = 600;
	float y = 5;

	enum Phase {
		Start,
		Play,
		Menu,
	};

	int NowPhase = Start;

	bool PlayFlg = false;

	uint32_t BGM;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
