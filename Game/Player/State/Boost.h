#pragma once 
#include "Game/State.h"
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

	// �u�[�X�g�����
	float m_boostPower = 7.5f;

	// �u�[�X�g����
	float m_boostTime = 0.2f;

	// �u�[�X�g�J�n���i��ł�������
	DirectX::SimpleMath::Vector2 m_velocity;

	// �̂���̃u�[�X�g����
	float m_totalTime;
};
