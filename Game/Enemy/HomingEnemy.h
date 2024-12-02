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

	HomingEnemy(IScene* scene, GameObject* target);
	~HomingEnemy();

	void Initialize();
	void Update(float elapsedTime);
	void CreateShader();
	void Render();
	void Finalize();


	void Shot();
	void ChangeState(State* state);

	void Collision(Collider* collider);

private:

	const int BULLET_COUNT = 10;

	std::vector<std::unique_ptr<Bullet>> m_bullet;
	State* m_state;
};

