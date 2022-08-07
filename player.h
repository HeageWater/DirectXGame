#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "playerbullet.h"
#include <cassert>
#include <memory>
#include <list>

class Player {
  public:
	void Initialize(Model* model,uint32_t textureHandle);
	void Draw(ViewProjection viewProjection);
	void Update();

  private:

	  void Trans();
	  void Rota();
	  void Scale();

	void Attack();

	void UpdateMatrix();

	Model* model = nullptr;
	
	Input* input = nullptr;

	DebugText* debugText = nullptr;

	WorldTransform playerW;

	uint32_t textureHandle = 0u;

	std::list<std::unique_ptr<PlayerBullet>> bullets;
};