#include "pch.h"
#include "EnemyMoveState.h"
#include "EnemyAttackState.h"
#include "Game/Enemy/Enemy.h"

// �R���X�g���N�^
EnemyMoveState::EnemyMoveState(Enemy* enemy)
	:
	m_totalTime{}
{
	m_enemy = enemy;
}

// ����������
void EnemyMoveState::Initialize()
{
	m_totalTime = 0;
}

// �X�V����
void EnemyMoveState::Update(float elapsedTime)
{

	m_totalTime += elapsedTime;

	// �s���ύX�C���^�[�o���`�F�b�N
	if (m_totalTime < MOVE_INTERVAL) return;

	// �v���C���[�Ƃ̋������v�Z
	float dis = (m_enemy->GetPosition() - m_enemy->GetTarget()->GetPosition()).Length();

	
	if (dis > SHOT_DISTANCE)
	{
		// ������΃v���C���[�ɋ߂Â�
		m_enemy->SetVelocity(DirectX::SimpleMath::Vector3{ 0,0,-SPEED });
		Initialize();
	}
	else
	{
		// �߂���΍U���X�e�[�g�ɑJ��
		m_enemy->SetVelocity(DirectX::SimpleMath::Vector3{ float(rand() % 2 * 2 - 1) * 0.1f,0,0 });
		m_enemy->ChangeState(m_enemy->GetAttackState());
	}
}

// �`�揈��
void EnemyMoveState::Render()
{
}

// �I������
void EnemyMoveState::Finalize()
{
}
