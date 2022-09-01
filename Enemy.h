#pragma once
#include "EnemyBullet.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include <list>
#include <memory>

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

  private:

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

	uint32_t textureHandle = 0u;

	void UpdateMatrix();

	void Fire(WorldTransform play, Model* bulletmodel);

	bool isDead_ = false;

};
