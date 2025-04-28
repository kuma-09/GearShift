#pragma once 
#include "Game/IState.h"
#include "Game/Parts/Part.h"
#include "Framework/InputManager.h"
#include "Game/Player/Player.h"

class Jump : public IState
{
public:
	Jump(Player* player);
	~Jump();

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();

private:
	Player* m_player;

	// ƒWƒƒƒ“ƒv—Í
	const float JUMPPOWER = 25.0f;

	// ˆÚ“®‚·‚é‹——£
	float m_velocity;
};
