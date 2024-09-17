#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"
#include "Game/GameObject.h"
#include "State/EnemyAttackState.h"
#include "State/EnemyMoveState.h"

class State;
class EnemyBullet;

class Enemy : public GameObject
{
public:

	~Enemy() = default;

	virtual void Initialize(GameObject* target) = 0;
	virtual void Update(float elapsedTime) = 0;
	virtual void Render() = 0;
	virtual void Finalize() = 0;

	void SetEnemyAttack(std::unique_ptr<EnemyAttackState> state) { m_attack = std::move(state); }
	void SetEnemyMove(std::unique_ptr<EnemyMoveState> state) { m_move = std::move(state); }
	void SetTarget(GameObject* target) { m_target = target; }

	EnemyAttackState* GetAttackState() { return m_attack.get(); }
	EnemyMoveState* GetMoveState() { return m_move.get(); }
	GameObject* GetTarget() { return m_target; }

	virtual void Shot() = 0;
	virtual void ChangeState(State* state) = 0;

	virtual void Collision(BoxCollider* collider) = 0;

private:
	GameObject* m_target;


	std::unique_ptr<EnemyAttackState> m_attack;
	std::unique_ptr<EnemyMoveState>   m_move;


};

