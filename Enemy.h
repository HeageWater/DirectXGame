#pragma once
#include "Audio.h"
#include "EnemyBullet.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include <list>
#include <memory>
#include <random>

enum class Phase {
	Move,
	Attack,
	Stay,
	Jump,
};

class Player;

class Enemy {
  public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Draw(ViewProjection viewProjection);
	void Update(WorldTransform play, Model* bulletmodel);

	Vector3 GetWorldPosition();

	//“–‚½‚è”»’è
	void OnCollision();

	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets; }

	//”­ŽËŠÔŠu
	static const int Kfire = 120;

	Player* player_ = nullptr;

	void SetPlayer(Player* player) { player_ = player; }

	WorldTransform EnemyW;

	std::list<std::unique_ptr<EnemyBullet>> bullets;

	void Jump();

	void Attack();

	void Dush();

	int hp = 100;

	void Reset();

	Input* input = nullptr;

	float GetRandom();

	int Maxhp = 150;

  private:
	float value;
	int a;

	uint32_t Shot = 0u;
	Audio* audio = nullptr;

	int muteki = 0;

	//ˆÚ“®ŒÀŠE
	const float kMoveLimitX = 48;
	const float kMoveLimitZ = 48;
	const float kMoveLimitY = 14;

	float Gravity = 0;
	float MaxGravity = 1.0f;

	float jump = 0;
	float Maxjump = 2;

	bool dush_flg = false;
	int dushcount = 0;

	int32_t Ktimer = 30;

	Phase phase_ = Phase::Stay;

	Model* model = nullptr;

	WorldTransform pate[10];

	uint32_t textureHandle = 0u;

	void UpdateMatrix(WorldTransform W);

	void Fire(WorldTransform play, Model* bulletmodel);

	bool isDead_ = false;

	int damege = 10;

	bool pate_flg = false;
};
