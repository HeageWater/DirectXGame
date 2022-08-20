#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include <list>
#include <memory>

enum class Phase {

};

class Player;

class Enemy {
  public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Draw(ViewProjection viewProjection);
	void Update();

	void SetPlayer(Player* player) { player_ = player; }
	Vector3 GetWorldPosition();

  private:
	Player* player_ = nullptr;

	Model* model = nullptr;

	WorldTransform EnemyW;

	uint32_t textureHandle = 0u;

	void UpdateMatrix();

	void Fire();
};
