#pragma once

#include "Game/GameObject.h"

class Graphics;
class InputManager;

class Camera :public GameObject
{
public:
	Camera();
	~Camera();

	void Initialize();
	void Update(float elapsedTime);
	void SetTarget(GameObject* player, GameObject* target);

	enum CameraState
	{
		Follow,
		Girth,
	};

private:
	Graphics* m_graphics;
	InputManager* m_inputManager;
	GameObject* m_target;
	GameObject* m_player;

	DirectX::SimpleMath::Vector3 m_targetPosition;

	float m_rotateX;
	float m_rotateY;


	// �J�����̍���
	const float CAMERA_HEIGHT = 2.5f;

	// �J�����̋���
	const float CAMERA_DISTANCE = 10.f;

	// �J�����̒Ǐ]�W��
	const float CAMERA_EYE_RATE = 0.5f;

	// �^�[�Q�b�g�ɑ΂��Ă̌W��
	const float CAMERA_TARGET_RATE = 0.1f;
};