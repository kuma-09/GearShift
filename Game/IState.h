#pragma once
#include "Game/GameObject.h"

class IState 
{
public:

	virtual ~IState() = default;

	virtual void Initialize() = 0;

	virtual void Update(float elapsedTime) = 0;

	virtual void Render() = 0;

	virtual void Finalize() = 0;

};