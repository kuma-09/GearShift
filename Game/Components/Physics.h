#pragma once
#include "IComponent.h"

class Collider;

/// <summary>
/// 重力コンポーネントクラス
/// </summary>
class Physics : public IComponent
{
public:

	// コンストラクタ
	Physics();
	// デストラクタ
	~Physics();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);

	// 重力をリセットする関数
	void Reset() { m_gravity = 0; }

private:
	// 重力の上限値
	const float MAX_GRAVITY = 0.5f;
	// 重力値
	float m_gravity;
	// 地面に触れているか
	bool m_onFloor;
};

