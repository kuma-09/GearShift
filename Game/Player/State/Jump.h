#pragma once 
#include "Game/State.h"
#include "Game/Parts/Part.h"
#include "Framework/InputManager.h"
#include "Game/Player/Player.h"

class Jump : public State
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

	// �W�����v��
	const float JUMPPOWER = 25.0f;

	// �ړ����鋗��
	float m_velocity;
};
