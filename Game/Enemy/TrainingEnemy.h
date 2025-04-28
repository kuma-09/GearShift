#pragma once
#include "Enemy.h"

class IState;

/// <summary>
/// �g���[�j���O�p�̃G�l�~�[
/// </summary>
class TrainingEnemy : public Enemy
{
public:
	// �R���X�g���N�^
	TrainingEnemy(Scene* scene);
	// �f�X�g���N�^
	~TrainingEnemy();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render();
	// �I������
	void Finalize();

	// �e�𔭎�
	void Shot();
	// �X�e�[�g��ύX
	void ChangeState(IState* state);
	// �����蔻��̏���
	void Collision(Collider* collider);

private:
	// HP���c���Ă��邩�`�F�b�N
	void CheckHP();
};

