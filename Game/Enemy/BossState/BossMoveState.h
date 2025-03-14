#pragma once
#include "Game/State.h"

class Enemy;

/// <summary>
/// �{�X�G�l�~�[�ړ��X�e�[�g
/// </summary>
class BossMoveState : public State
{
public:
	// �R���X�g���N�^
	BossMoveState(Enemy* enemy);
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render();
	// �I������
	void Finalize();
private:
	// �^�b�N�����K�g�����O��ԂɃ����_���őJ��
	void RandomTackleOrGatlingState();
private:
	Enemy* m_enemy;

	// �ړ��X�s�[�h
	const float SPEED = 0.2f;
	// �ړ������̕ύX�Ԋu
	const float MOVE_INTERVAL = 2.0f;
	// �~�T�C�����˃X�e�[�g�J�ڋ���
	const float MISSILE_DISTANCE = 80.0f;
	// �ړ��X�e�[�g�̌o�ߎ���
	float m_totalTime;
};