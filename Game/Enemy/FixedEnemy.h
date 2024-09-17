#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"
#include "Enemy.h"

class State;
class EnemyBullet;
class EnemyAttackState;
class EnemyMoveState;

class FixedEnemy : public Enemy
{
public:

	FixedEnemy(IScene* scene);
	~FixedEnemy();

	void Initialize(GameObject* target);
	void Update(float elapsedTime);
	void Render();
	void Finalize();


	void Shot();
	void ChangeState(State* state);

	void Collision(BoxCollider* collider);

private:

	std::unique_ptr<EnemyBullet> m_bullet;

	State* m_state;
};

