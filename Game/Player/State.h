#pragma once
#include "Game/GameObject.h"

class State : public GameObject
{
public:

	virtual void Initialize(GameObject* gameobject) = 0;

	virtual void Update(float elapsedTime) = 0;

	virtual void Render() = 0;

	virtual void Finalize() = 0;

	virtual void SetTarget(GameObject* gameobject) = 0;

	virtual void ChageState(State* state) = 0;
};