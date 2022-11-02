#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Syodome {
  public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Draw(ViewProjection viewProjection);
	void Update();
	~Syodome();

  private:
	WorldTransform worldTransform;

	Model* model = nullptr;

	uint32_t textureHandle = 0u;
};