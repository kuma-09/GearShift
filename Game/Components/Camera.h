#pragma once
#include "Game/Components/IComponent.h"
#include "Framework/Graphics.h"
#include "Game/GameObject.h"

class Camera : public IComponent
{
public:
	Camera();
	~Camera();

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();

	void shake();
	void SetTarget(GameObject* player, GameObject* enemy);

private:
	Graphics* m_graphics;
	
	// �J�����̍���
	const float CAMERA_HEIGHT	    =  2.5f;

	// �J�����̋���
	const float CAMERA_DISTANCE     = 15.0f;

	// �J�����̒Ǐ]�W��
	const float CAMERA_EYE_RATE     = 0.05f;

	// �^�[�Q�b�g�ɑ΂��Ă̌W��
	const float CAMERA_TARGET_RATE  =  0.1f;

	// ��񓖂���̉�ʗh��̎���
	const float SHAKE_TIME = 1.0f;

	// ��ʗh��W��
	const float SHAKE_RATE = 1.0f;

	// ��ʗh��W��
	float m_shakeRate;

	// ��ʗh��o�ߎ���
	float m_shakeTime;



	GameObject* m_player;
	GameObject* m_enemy;

	DirectX::SimpleMath::Vector3 m_eyePosition;
	DirectX::SimpleMath::Vector3 m_targetPosition;
};

