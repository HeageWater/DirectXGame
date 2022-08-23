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
	void Update();

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

  private:
	int32_t Ktimer =10;

	Phase phase_ = Phase::Stay;

	Model* model = nullptr;

	uint32_t textureHandle = 0u;

	void UpdateMatrix();

	void Fire();

	bool isDead_ = false;
};
