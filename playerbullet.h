#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include <cassert>

class PlayerBullet {
  public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

  //private:
	WorldTransform bulletW;
	Model* model;
	uint32_t textureHandle = 0u;

	//���x
	Vector3 velocity;

	//����
	static const int32_t Time = 60 * 5;

	//�f�X�^�C�}�[
	int32_t deathTimer = Time;

	//�f�X�t���O
	bool isDead_ = false;

	bool IsDead() const { return isDead_; }
};