#pragma once
#include "Game/GameObject.h"

/// <summary>
/// �������̃g���C��
/// </summary>
class BurnerVertical : public GameObject
{
public:
	// �R���X�g���N�^
	BurnerVertical();
	// �f�X�g���N�^
    ~BurnerVertical();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);


private:
};
