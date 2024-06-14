#pragma once 
#include "State.h"
#include "Game/Parts/IPart.h"
#include "Framework/InputManager.h"
#include "Game/Player/Player.h"

class Boost : public State
{
public:
	Boost();
	~Boost();

	void Initialize(Player* player);
	void Update(float elapsedTime);
	void Render();
	void Finalize();

	void SetTarget(GameObject* target);

private:
	Player* m_player;

	InputManager* m_inputManager;

	GameObject* m_target;

	// �u�[�X�g�����
	float m_boostPower = 10.0f;

	// �u�[�X�g����
	float m_boostTime = 0.5f;

	// �u�[�X�g�J�n���i��ł�������
	DirectX::SimpleMath::Vector3 m_velocity;

	// �̂���̃u�[�X�g����
	float m_totalTime;
};
