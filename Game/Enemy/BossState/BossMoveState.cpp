#include "pch.h"
#include "BossMoveState.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/BossEnemy.h"

// �R���X�g���N�^
BossMoveState::BossMoveState(Enemy* enemy)
{
	m_enemy = enemy;
}

// ����������
void BossMoveState::Initialize()
{
	m_totalTime = 0;
}

// �X�V����
void BossMoveState::Update(float elapsedTime)
{

	m_totalTime += elapsedTime;

	// �v���C���[����̋���
	float dis = (m_enemy->GetPosition() - m_enemy->GetTarget()->GetPosition()).Length();

	// �ړ������̕ύX
	if (m_totalTime < MOVE_INTERVAL) return;

	// ���������ꂽ��߂Â��Ȃ���~�T�C������
	if (dis > MISSILE_DISTANCE)
	{
		m_enemy->SetVelocity(DirectX::SimpleMath::Vector3{ 0,0,-SPEED });
		Initialize();
		m_enemy->ChangeState(static_cast<BossEnemy*>(m_enemy)->GetMissileState());
		m_totalTime = 0;
	}
	else
	{
		RandomTackleOrGatlingState();
	}
}

// �`�揈��
void BossMoveState::Render()
{
}

// �I������
void BossMoveState::Finalize()
{
}

/// <summary>
/// �^�b�N�����K�g�����O��ԂɃ����_���őJ��
/// </summary>
void BossMoveState::RandomTackleOrGatlingState()
{
	switch (rand() % 2)
	{
	case 0:
		m_enemy->SetVelocity(DirectX::SimpleMath::Vector3{ float(rand() % 2 * 2 - 1) * SPEED,0,0 });
		m_enemy->ChangeState(m_enemy->GetAttackState());
		m_totalTime = 0;
		break;
	case 1:
		m_enemy->ChangeState(static_cast<BossEnemy*>(m_enemy)->GetTackleState());
		m_totalTime = 0;
	default:
		break;
	}
}
