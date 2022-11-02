#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

class EnemyBullet {
  public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity_);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	//当たり判定
	void OnCollision();

	Vector3 GetWorldPosition();

	// private:
	WorldTransform bulletW;
	Model* model;
	uint32_t textureHandle = 0u;

	//速度
	Vector3 velocity;

	//寿命
	static const int32_t Time = 60 * 5;

	//デスタイマー
	int32_t deathTimer = Time;

	//デスフラグ
	bool isDead_ = false;

	bool IsDead() const { return isDead_; }
};