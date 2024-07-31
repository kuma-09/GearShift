#pragma once 
#include "Game/State.h"
#include "Game/Parts/Part.h"
#include "Framework/InputManager.h"
#include "Game/Player/Player.h"
#include "Game/Object/Sword.h"

class Attack : public State
{
public:
	Attack(Player* player);
	~Attack();

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();


private:
	Player* m_player;

	// ブーストする力
	float m_boostPower = 1.0f;

	// ブースト時間
	float m_boostTime = 0.5f;

	// ブースト開始時進んでいた方向
	DirectX::SimpleMath::Vector3 m_velocity;

	// ブースト開始時向いていた方向
	DirectX::SimpleMath::Quaternion m_quaternion;

	// のこりのブースト時間
	float m_totalTime;

	std::unique_ptr<Sword> m_sword;
};
