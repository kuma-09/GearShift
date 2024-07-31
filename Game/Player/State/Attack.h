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

	// �u�[�X�g�����
	float m_boostPower = 1.0f;

	// �u�[�X�g����
	float m_boostTime = 0.5f;

	// �u�[�X�g�J�n���i��ł�������
	DirectX::SimpleMath::Vector3 m_velocity;

	// �u�[�X�g�J�n�������Ă�������
	DirectX::SimpleMath::Quaternion m_quaternion;

	// �̂���̃u�[�X�g����
	float m_totalTime;

	std::unique_ptr<Sword> m_sword;
};
