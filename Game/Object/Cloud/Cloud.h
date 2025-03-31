#pragma once
#include "Game/GameObject.h"
#include "Framework/Resources.h"
/// <summary>
/// ‰_ƒNƒ‰ƒX
/// </summary>
class Cloud : public GameObject
{
public:
	Cloud(Resources::ModelType modelType);
	~Cloud();

	void Initialize();
	void Update(float elapsedTime);

private:

};