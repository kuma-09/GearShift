#pragma once 
#include "State.h"
#include "Game/Parts/Part.h"
#include "Framework/InputManager.h"
#include "Game/Player/Player.h"

class Boost : public State
{
public:
	Boost(Player* player);
	~Boost();

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();


private:
	Player* m_player;

	// ブーストする力
	float m_boostPower = 10.0f;

	// ブースト時間
	float m_boostTime = 0.5f;

	// ブースト開始時進んでいた方向
	DirectX::SimpleMath::Vector3 m_velocity;

	// のこりのブースト時間
	float m_totalTime;
};
