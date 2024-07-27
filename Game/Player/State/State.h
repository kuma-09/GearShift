#pragma once
#include "Game/GameObject.h"

class State 
{
public:

	virtual void Initialize() = 0;

	virtual void Update(float elapsedTime) = 0;

	virtual void Render() = 0;

	virtual void Finalize() = 0;

};