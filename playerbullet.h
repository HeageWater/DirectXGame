#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include <cassert>

class PlayerBullet {
  public:
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

  private:
	WorldTransform worldTransform;
	Model* model;
	uint32_t textureHandle = 0u;
};