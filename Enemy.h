#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include <list>
#include <memory>

enum class Phase {

};

class Enemy {
  public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Draw(ViewProjection viewProjection);
	void Update();

  private:
	Model* model = nullptr;

	WorldTransform EnemyW;

	uint32_t textureHandle = 0u;

	void UpdateMatrix();
};
#pragma once
