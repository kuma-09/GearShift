#pragma once

#include "Game/GameObject.h"

/// <summary>
/// �|�C���g���C�g�I�u�W�F�N�g
/// </summary>
class Light : public GameObject
{
public:
	// �R���X�g���N�^
	Light(Scene* scene);
	// �f�X�g���N�^
	~Light();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedtime);
	// �`�揈��
	void Render();

private:

};
