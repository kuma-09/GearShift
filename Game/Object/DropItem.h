#pragma once
#include "Game/GameObject.h"
/// <summary>
/// �h���b�v�A�C�e���N���X
/// </summary>
class DropItem :public GameObject
{
public:
	// �R���X�g���N�^
	DropItem(IScene* scene);
	// �f�X�g���N�^
	~DropItem();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);
	// �����蔻��̏���
	void Collision(Collider* collider);
private:
	// ���f���̃T�C�Y
	const float MODEL_SIZE = 0.1f;
	// �v���C���[��HP���񕜂���l
	const int   RECOVERY_HP_POINT = 5;
};
