#pragma once
#include "Game/State.h"

class Enemy;

/// <summary>
/// �{�X�̒e�A�˃X�e�[�g
/// </summary>
class BossGatlingState : public State
{
public:
	BossGatlingState(Enemy* enemy);
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

	// �e�A�˃X�e�[�g�̎���
	const float STATE_INTERVAL = 1.0f;
	// �e�̔��ˊԊu
	const float SHOT_INTERVAL = 0.1f;
	// �e��Interval����
	float m_bulletInterval;
	// �X�e�[�g�̌o�ߎ���
	float m_totalTime;
};
