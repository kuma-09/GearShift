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

	void SetTarget(GameObject* player, GameObject* enemy);

private:
	Graphics* m_graphics;
	
	// カメラの高さ
	const float CAMERA_HEIGHT	    =  2.5f;

	// カメラの距離
	const float CAMERA_DISTANCE     = 15.0f;

	// カメラの追従係数
	const float CAMERA_EYE_RATE     = 0.05f;

	// ターゲットに対しての係数
	const float CAMERA_TARGET_RATE  =  0.1f;

	GameObject* m_player;
	GameObject* m_enemy;

	DirectX::SimpleMath::Vector3 m_eyePosition;
	DirectX::SimpleMath::Vector3 m_targetPosition;
};

