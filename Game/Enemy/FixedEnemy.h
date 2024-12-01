#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"
#include "Enemy.h"

class State;
class FixedEnemyBullet;

class FixedEnemy : public Enemy
{
public:

	FixedEnemy(IScene* scene);
	~FixedEnemy();

	void Initialize(GameObject* target);
	void Update(float elapsedTime);
	void CreateShader();
	void Render();
	void Finalize();


	void Shot();
	void ChangeState(State* state);

	void Collision(BoxCollider* collider);

private:

	std::unique_ptr<FixedEnemyBullet> m_bullet;

	State* m_state;
};

