#pragma once
#include "Component.h"
#include "Framework/InputManager.h"

/// <summary>
/// 入力コンポーネントクラス
/// </summary>
class InputComponent : public Component
{
public:
	// コンストラクタ
	InputComponent();
	// デストラクタ
	~InputComponent();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// 入力方向を取得
	DirectX::SimpleMath::Vector3 GetVelocity() { return m_velocity; }

private:
	InputManager* m_inputManager;

	// 入力方向保存用
	DirectX::SimpleMath::Vector3 m_velocity;

	// 最大速度
	const float MAX_SPEED = 0.95f;
};

