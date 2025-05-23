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

	// ジャンプ力
	const float JUMPPOWER = 25.0f;

	// 移動する距離
	float m_velocity;
};
