#pragma once
#include "Game/IState.h"

/// <summary>
/// �g���[�j���O�p�̓G�X�e�[�g
/// </summary>
class TrainingIdolState : public IState
{
public:
	// �R���X�g���N�^
	TrainingIdolState();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render();
	// �I������
	void Finalize();
private:
};
