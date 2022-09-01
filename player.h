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

	int hp = 100;

	int Maxhp = 150;

	void Reset();

	bool dush_flg = false;
	int dushcount = 0;

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
	float Maxjump = 2.5;

	

	Model* model = nullptr;
	Model* bulletmodel = nullptr;

	Audio* audio = nullptr;

	Input* input = nullptr;

	DebugText* debugText = nullptr;

	uint32_t textureHandle = 0u;

	int32_t Ktimer = 30;

	uint32_t ShotO = 0u;
	uint32_t DashO = 0u;
	uint32_t BomO = 0u;
	uint32_t JumpO = 0u;

	std::list<std::unique_ptr<PlayerBullet>> bullets;

	float centerV;

	Vector3 center;

	Vector3 centerDirection;

	Vector3 rightV;

	Vector3 temp = {0, 1, 0};

	int muteki = 30;

	bool isDead_ = false;

	int damege = 10;
};