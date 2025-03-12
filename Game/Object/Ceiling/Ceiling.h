#pragma once
#include "Game/GameObject.h"

class Ceiling : public GameObject
{
public:
	Ceiling();
	~Ceiling();
	void Initialize();
	void Update(float elapsedtime);
	void Render();
private:

};