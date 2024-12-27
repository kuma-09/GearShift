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


	// カメラの高さ
	const float CAMERA_HEIGHT = 2.5f;

	// カメラの距離
	const float CAMERA_DISTANCE = 10.f;

	// カメラの追従係数
	const float CAMERA_EYE_RATE = 0.5f;

	// ターゲットに対しての係数
	const float CAMERA_TARGET_RATE = 0.1f;
};