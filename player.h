#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "Audio.h"
#include "WorldTransform.h"
#include "playerbullet.h"
#include <cassert>
#include <list>
#include <memory>

class Player {
  public:
	void Initialize(Model* model, Model* model2, uint32_t textureHandle);
	void Draw(ViewProjection viewProjection);
	void Update(WorldTransform enemy, ViewProjection viewProjection);

	Vector3 GetWorldPosition();

	Vector3 GetBulletWorldPosition();

	//“–‚½‚è”»’è
	void OnCollision();

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets; }

	WorldTransform playerW;

	//XINPUT_STATE joyState;

  private:
	void Trans();
	void Rota();
	void Scale();

	void Jump();

	void Attack(WorldTransform enemy);

	void Dush();

	void UpdateMatrix();

	float Gravity = 0;
	float MaxGravity = 1.0f;

	float jump = 0;
	float Maxjump = 2;

	bool dush_flg = false;
	int dushcount = 0;

	Model* model = nullptr;
	Model* bulletmodel = nullptr;

	Audio* audio = nullptr;

	Input* input = nullptr;

	DebugText* debugText = nullptr;

	uint32_t textureHandle = 0u;

	uint32_t Shot = 0u;

	std::list<std::unique_ptr<PlayerBullet>> bullets;

	float centerV;

	Vector3 center;

	Vector3 centerDirection;

	Vector3 rightV;
};