#pragma once
#include "Game/GameObject.h"

class TitleCamera : public GameObject
{
public:
	TitleCamera();
	~TitleCamera();

	void Initialize(GameObject* target);
	void Update(float elapsedTime);

private:
	GameObject* m_target;

};
