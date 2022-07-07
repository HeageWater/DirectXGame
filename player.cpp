#include "player.h"

void Player::Initialize() {}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model = model;

	this->textureHandle = textureHandle;

	input = Input::GetInstance();
	debugText = DebugText::GetInstance();

	worldTransform.Initialize();
}

void Player::Draw() {}

void Player::Update() 
{
	Vector3 move = {0, 0, 0}; 


}
