#include "player.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model = model;

	this->textureHandle = textureHandle;

	input = Input::GetInstance();
	debugText = DebugText::GetInstance();

	worldTransform.Initialize();
}

void Player::Draw(ViewProjection viewProjection) { 
	model->Draw(worldTransform, viewProjection, textureHandle);

	debugText->SetPos(50, 110);
	debugText->Print(
	  "trans:%f,%f,%f",
		worldTransform.translation_.x, 
		worldTransform.translation_.y,
		worldTransform.translation_.z);
}

void Player::Update() 
{
	const int speed = 12;

	Vector3 move = {0, 0, 0}; 

	if (input->PushKey(DIK_UP)) {
		move.y = speed;
	}

	if (input->PushKey(DIK_DOWN)) {
		move.y = -speed;
	}

	if (input->PushKey(DIK_RIGHT)) {
		move.x = speed;
	}

	if (input->PushKey(DIK_LEFT)) {
		move.x = -speed;
	}

	worldTransform.translation_.x += move.x;
	worldTransform.translation_.y += move.y;
	worldTransform.translation_.z += move.z;

	worldTransform.TransferMatrix();
}
