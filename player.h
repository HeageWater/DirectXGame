#pragma once

#include "Model.h"
#include "WorldTransform.h"

class Player {
  public:
	void Initialize();
	void Initialize(Model* model,uint32_t textureHandle);
	void Draw();
	void Update();

  private:
	Model* model = nullptr;
	
	WorldTransform worldTransform;

	uint32_t textureHandle = 0u;
};