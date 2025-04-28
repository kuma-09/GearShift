#pragma once
#include "Component.h"

/// <summary>
/// ポイントライトコンポーネントクラス
/// </summary>
class PointLight : public Component
{
public:
	// コンストラクタ
	PointLight();
	// デストラクタ
	~PointLight();
	// 初期化処理
	void Initialize(DirectX::SimpleMath::Vector3 = {1,1,1});
	// 更新処理
	void Update(float elapsedTime);
	// カラーをリセット
	void ClearColor();
	// ポイントライトの色を取得
	DirectX::SimpleMath::Vector3 GetColor() { return m_nowColor; }

private:
	// カラー
	DirectX::SimpleMath::Vector3 m_targetColor;
	DirectX::SimpleMath::Vector3 m_nowColor;
	// 経過時間
	float m_nowTime;
};

