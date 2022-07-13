#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "playerbullet.h"
#include <cassert>

class Player {
  public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Draw(ViewProjection viewProjection);
	void Update();

  private:
	void Attack();

	void Effect();

	void Trans();
	void Scale();
	void Rota();

	Model* model = nullptr;

	Input* input = nullptr;

	DebugText* debugText = nullptr;

	WorldTransform worldTransform;

	uint32_t textureHandle = 0u;

	// PlayerBullet* bullet = nullptr;

	bool effectF = false;
};