#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include <cassert>

class Player {
  public:
	void Initialize();
	void Initialize(Model* model,uint32_t textureHandle);
	void Draw();
	void Update();

  private:
	Model* model = nullptr;
	
	Input* input = nullptr;

	DebugText* debugText = nullptr;

	WorldTransform worldTransform;

	uint32_t textureHandle = 0u;
};