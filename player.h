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
	void Update();

	Vector3 GetWorldPosition();

	//“–‚½‚è”»’è
	void OnCollision();

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets; }

	WorldTransform playerW;

	XINPUT_STATE joyState;

	Vector3 GetBulletWorldPosition();

  private:
	void Trans();
	void Rota();
	void Scale();

	void Jump();

	void Attack();

	void Dush();

	void UpdateMatrix();

	float Gravity = 0;
	float MaxGravity = 1.0f;

	float jump = 0;
	float Maxjump = 2;

	bool dush_flg = false;
	int dushcount = 0;


	Model* model = nullptr;

	Input* input = nullptr;

	DebugText* debugText = nullptr;

	uint32_t textureHandle = 0u;

	std::list<std::unique_ptr<PlayerBullet>> bullets;
};