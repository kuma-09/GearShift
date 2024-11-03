#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"
#include "Enemy.h"

class State;
class Bullet;
class EnemyAttackState;
class EnemyMoveState;

class HomingEnemy : public Enemy
{
public:

	HomingEnemy(IScene* scene);
	~HomingEnemy();

	void Initialize(GameObject* target);
	void Update(float elapsedTime);
	void CreateShader();
	void Render();
	void Finalize();


	void Shot();
	void ChangeState(State* state);

	void Collision(BoxCollider* collider);

private:

	std::unique_ptr<Bullet> m_bullet;

	State* m_state;
};

