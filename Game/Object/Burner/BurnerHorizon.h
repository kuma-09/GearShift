#pragma once
#include "Game/GameObject.h"

// �c�����̃g���C��
class BurnerHorizon : public GameObject
{
public:
	// �R���X�g���N�^
	BurnerHorizon();
	// �f�X�g���N�^
	~BurnerHorizon();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);

private:
};