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

	void shake();

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


	// ƒJƒƒ‰‚Ì‚‚³
	const float CAMERA_HEIGHT = 2.5f;

	// ƒJƒƒ‰‚Ì‹——£
	const float CAMERA_DISTANCE = 10.f;

	// ƒJƒƒ‰‚Ì’Ç]ŒW”
	const float CAMERA_EYE_RATE = 0.5f;

	// ƒ^[ƒQƒbƒg‚É‘Î‚µ‚Ä‚ÌŒW”
	const float CAMERA_TARGET_RATE = 0.1f;

	// ˆê‰ñ“–‚½‚è‚Ì‰æ–Ê—h‚ê‚ÌŠÔ
	const float SHAKE_TIME = 0.1f;

	// ‰æ–Ê—h‚êŒW”
	const float SHAKE_RATE = 0.3f;

	// ‰æ–Ê—h‚êŒW”
	float m_shakeRate;

	// ‰æ–Ê—h‚êŒo‰ßŠÔ
	float m_shakeTime;
};