#pragma once
#include "Game/State.h"

class Enemy;

/// <summary>
/// �{�X�̃~�T�C�����˃X�e�[�g
/// </summary>
class BossMissileState : public State
{
public:
	// �R���X�^���g
	BossMissileState(Enemy* enemy);
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

	// �~�T�C�����ˊԊu
	const float SHOT_INTERVAL = 0.3f;
	// �X�e�[�g�̌o�ߎ���
	float m_totalTime;
};
