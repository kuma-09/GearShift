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
	
	// ƒJƒƒ‰‚Ì‚‚³
	const float CAMERA_HEIGHT	    =  2.5f;

	// ƒJƒƒ‰‚Ì‹——£
	const float CAMERA_DISTANCE     = 15.0f;

	// ƒJƒƒ‰‚Ì’Ç]ŒW”
	const float CAMERA_EYE_RATE     = 0.05f;

	// ƒ^[ƒQƒbƒg‚É‘Î‚µ‚Ä‚ÌŒW”
	const float CAMERA_TARGET_RATE  =  0.1f;

	// ˆê‰ñ“–‚½‚è‚Ì‰æ–Ê—h‚ê‚ÌŠÔ
	const float SHAKE_TIME = 1.0f;

	// ‰æ–Ê—h‚êŒW”
	const float SHAKE_RATE = 1.0f;

	// ‰æ–Ê—h‚êŒW”
	float m_shakeRate;

	// ‰æ–Ê—h‚êŒo‰ßŠÔ
	float m_shakeTime;



	GameObject* m_player;
	GameObject* m_enemy;

	DirectX::SimpleMath::Vector3 m_eyePosition;
	DirectX::SimpleMath::Vector3 m_targetPosition;
};

