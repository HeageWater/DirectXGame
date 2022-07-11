#include "playerbullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position) {

	// NULL�|�C���^�`�F�b�N
	assert(model);

	//���f��
	this->model = model;

	//�摜�f�[�^
	textureHandle = TextureManager::Load("black.png");

	//������
	worldTransform.Initialize();

	//�ꏊ���
	worldTransform.translation_ = position;
}

void PlayerBullet::Update() {
	// worldTransform�X�V

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model->Draw(worldTransform, viewProjection, textureHandle);
}
