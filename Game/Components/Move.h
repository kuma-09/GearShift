#pragma once
#include "Component.h"
#include "Framework/InputManager.h"

/// <summary>
/// 移動を反映させるコンポーネントクラス
/// </summary>
class Move : public Component
{
public:
	// コンストラクタ
	Move();
	// デストラクタ
	~Move();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// 移動量を取得
	DirectX::SimpleMath::Vector3 GetVelocity();
	// 移動入力されているか
	bool GetIsMove() { return m_isMove; }

private:
	const float MAX_SPEED = 0.95f;

private:
	InputManager* m_inputManager;
	DirectX::SimpleMath::Vector3 m_velocity;
	// 移動入力の有無
	bool m_isMove;


};

