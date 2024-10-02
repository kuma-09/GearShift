#pragma once
#include "Game/GameObject.h"

class Wall :public GameObject
{
public:
	Wall() {};
	virtual ~Wall() = default;

	virtual void Initialize() = 0;
	virtual void Update(float elapsedtime) = 0;
	virtual void Render() = 0;
};
