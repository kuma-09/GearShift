#pragma once
#include "Game/Components/IComponent.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
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
	DirectX::SimpleMath::Vector3 GetTargetPosition() { return m_targetPosition; }
	DirectX::SimpleMath::Quaternion GetCameraQuaternion(){ return m_quaternion; };


private:
	Graphics* m_graphics;
	
	// ƒJƒƒ‰‚Ì‚‚³
	const float CAMERA_HEIGHT	    =  2.5f;

	// ƒJƒƒ‰‚Ì‹——£
	const float CAMERA_DISTANCE     =  10.f;

	// ƒJƒƒ‰‚Ì’Ç]ŒW”
	const float CAMERA_EYE_RATE     = 0.05f;

	// ƒ^[ƒQƒbƒg‚É‘Î‚µ‚Ä‚ÌŒW”
	const float CAMERA_TARGET_RATE  =  0.1f;

	// ˆê‰ñ“–‚½‚è‚Ì‰æ–Ê—h‚ê‚ÌŠÔ
	const float SHAKE_TIME = 0.1f;

	// ‰æ–Ê—h‚êŒW”
	const float SHAKE_RATE = 2.0f;

	// ‰æ–Ê—h‚êŒW”
	float m_shakeRate;

	// ‰æ–Ê—h‚êŒo‰ßŠÔ
	float m_shakeTime;

	

	GameObject* m_player;
	GameObject* m_enemy;

	DirectX::SimpleMath::Vector3 m_eyePosition;
	DirectX::SimpleMath::Vector3 m_targetPosition;

	DirectX::SimpleMath::Quaternion m_quaternion;

	float m_rotateX;
	float m_rotateY;

};

