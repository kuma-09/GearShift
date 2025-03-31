#pragma once

#include "Game/GameObject.h"

class Graphics;
class InputManager;

/// <summary>
/// カメラオブジェクト
/// </summary>
class Camera :public GameObject
{
public:
	// コンストラクタ
	Camera();
	// デストラクタ
	~Camera();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// ターゲットをセット
	void SetTarget(GameObject* player, GameObject* target);
	// カメラを揺らす
	void shake();
	// カメラの状態
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

	// 一回当たりの画面揺れの時間
	const float SHAKE_TIME = 0.1f;

	// 画面揺れ係数
	const float SHAKE_RATE = 1.0f;

	// 画面揺れ係数
	float m_shakeRate;

	// 画面揺れ経過時間
	float m_shakeTime;
};