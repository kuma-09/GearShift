#pragma once
#include "Game/IState.h"

class Enemy;

/// <summary>
/// �{�X�̓ˌ��X�e�[�g
/// </summary>
class BossTackleState : public IState
{
public:
	// �R���X�g���N�^
	BossTackleState(Enemy* enemy);
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render();
	// �I������
	void Finalize();
private:
	Enemy* m_enemy;

	// �X�e�[�g�̎���
	const float STATE_INTERVAL = 1.0f;
	// �ړ��X�s�[�h
	const float SPEED = 0.3f;
	// �X�e�[�g�̌o�ߎ���
	float m_totalTime;
};
