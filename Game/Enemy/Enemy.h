#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"
#include "Game/GameObject.h"
#include "Game/Player/Player.h"
#include "State/EnemyAttackState.h"
#include "State/EnemyMoveState.h"

class State;
class EnemyBullet;

/// <summary>
/// �G�̊��N���X
/// </summary>
class Enemy : public GameObject
{
public:
	// �f�X�g���N�^
	~Enemy() = default;

	virtual void Initialize() = 0;
	virtual void Update(float elapsedTime) = 0;
	virtual void Render() = 0;
	virtual void Finalize() = 0;

	// �U���X�e�[�g
	void SetEnemyAttack(std::unique_ptr<State> state) { m_attack = std::move(state); }
	State* GetAttackState() { return m_attack.get(); }
	// �ړ��X�e�[�g
	void SetEnemyMove(std::unique_ptr<State> state) { m_move = std::move(state); }
	State* GetMoveState() { return m_move.get(); }
	// �^�[�Q�b�g
	void SetTarget(GameObject* target) { m_target = target; }
	GameObject* GetTarget() { return m_target; }

	// �e���ˊ֐�
	virtual void Shot() = 0;
	// �X�e�[�g�̕ύX
	virtual void ChangeState(State* state) = 0;
	// �����蔻��̏���
	virtual void Collision(Collider* collider) = 0;

private:
	GameObject* m_target;
	std::unique_ptr<State> m_attack;
	std::unique_ptr<State>   m_move;
};

