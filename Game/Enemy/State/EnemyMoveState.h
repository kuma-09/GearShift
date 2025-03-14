#pragma once
#include "Game/State.h"

class Enemy;

/// <summary>
/// �G�̈ړ��X�e�[�g
/// </summary>
class EnemyMoveState : public State
{
public:
	// �R���X�g���N�^
	EnemyMoveState(Enemy* enemy);
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

	// �ړ����x
	const float SPEED = 0.1f;
	// �ړ��X�e�[�g�̃C���^�[�o��
	const float MOVE_INTERVAL = 3.0f;
	// �U���X�e�[�g�ύX����
	const float SHOT_DISTANCE = 40.0f;
	// �X�e�[�g�̌o�ߎ���
	float m_totalTime;
};