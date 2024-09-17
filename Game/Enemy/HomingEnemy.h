#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"
#include "Enemy.h"

class State;
class EnemyBullet;
class EnemyAttackState;
class EnemyMoveState;

class HomingEnemy : public Enemy
{
public:

	HomingEnemy(IScene* scene);
	~HomingEnemy();

	void Initialize(GameObject* target);
	void Update(float elapsedTime);
	void Render();
	void Finalize();

	EnemyAttackState* GetAttackState() { return m_attack.get(); }
	EnemyMoveState* GetMoveState() { return m_move.get(); }
	GameObject* GetTarget() { return m_target; }

	void Shot();
	void ChangeState(State* state);

	void Collision(BoxCollider* collider);

private:
	GameObject* m_target;

	std::unique_ptr<EnemyBullet> m_bullet;

	State* m_state;

	std::unique_ptr<EnemyAttackState> m_attack;
	std::unique_ptr<EnemyMoveState>   m_move;
};

