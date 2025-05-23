#pragma once 
#include "Game/IState.h"
#include "Game/Parts/Part.h"
#include "Framework/InputManager.h"
#include "Game/Player/Player.h"
#include "Game/Shader/Shader.h"

class Boost : public IState
{
public:
	Boost(Player* player);
	~Boost();

	void Initialize();
	void Update(float elapsedTime);
	void Render() {};
	void Finalize();


private:
	Player* m_player;
	std::unique_ptr<Shader> m_shader;

	// ブーストする力
	float m_boostPower = 7.5f;

	// ブースト時間
	float m_boostTime = 0.2f;

	// ブースト開始時進んでいた方向
	DirectX::SimpleMath::Vector2 m_velocity;

	// のこりのブースト時間
	float m_totalTime;

	DirectX::SimpleMath::Matrix m_world;
};
