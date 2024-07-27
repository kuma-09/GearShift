#pragma once 
#include "State.h"
#include "Game/Parts/Part.h"
#include "Framework/InputManager.h"
#include "Game/Player/Player.h"

class Idol : public State
{
public:
	Idol(Player* player);
	~Idol();

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();


private:
	Player* m_player;
};
