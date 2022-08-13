#include "Syodome.h"

Syodome::~Syodome() {}

void Syodome::Initialize(Model* model, uint32_t textureHandle) {
	this->model = model;

	worldTransform.Initialize();

	worldTransform.translation_ = {0, 0, 0};

	this->textureHandle = textureHandle;
}
void Syodome::Draw(ViewProjection viewProjection) {
	model->Draw(worldTransform, viewProjection, textureHandle);
}
void Syodome::Update() {}