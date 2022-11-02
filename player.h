#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "playerbullet.h"
#include <cassert>
#include <list>
#include <memory>

class Player {
  public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Draw(ViewProjection viewProjection);
	void Update(ViewProjection viewProjection);

	Vector3 GetWorldPosition();

	WorldTransform playerWorldTransform;

	void Reset();

  private:
	void Trans();
	void Rota();
	void Scale();

	void Jump();

	void UpdateMatrix();
	void UpdateMatrix(WorldTransform W);

	float Gravity = 0;
	float MaxGravity = 1.0f;

	float jump = 0;
	float Maxjump = 2.5;

	Model* model = nullptr;

	Input* input = nullptr;

	DebugText* debugText = nullptr;

	uint32_t textureHandle = 0u;

	bool isDead_ = false;
};