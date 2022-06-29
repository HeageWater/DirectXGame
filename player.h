#pragma once

#include "Model.h"
#include "WorldTransform.h"

class Player {
  public:
	void Initialize();
	void Draw();
	void Update();

  private:
	Model* model = nullptr;
	


	WorldTransform* worldTransform;
};