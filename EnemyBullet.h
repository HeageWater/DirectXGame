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

	//�����蔻��
	void OnCollision();

	Vector3 GetWorldPosition();

	// private:
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