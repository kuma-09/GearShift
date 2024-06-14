#pragma once
#include "Game/GameObject.h"

class Player;

class State : public GameObject
{
public:

	virtual void Initialize(Player* player) = 0;

	virtual void Update(float elapsedTime) = 0;

	virtual void Render() = 0;

	virtual void Finalize() = 0;

	virtual void SetTarget(GameObject* gameobject) = 0;
};