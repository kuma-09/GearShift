#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"
#include "Enemy.h"

class State;

class TrainingEnemy : public Enemy
{
public:

	TrainingEnemy(IScene* scene, GameObject* target);
	~TrainingEnemy();

	void Initialize();
	void Update(float elapsedTime);
	void CreateShader();
	void Render();
	void Finalize();


	void Shot();
	void ChangeState(State* state);

	void Collision(Collider* collider);

private:
	State* m_state;
};

