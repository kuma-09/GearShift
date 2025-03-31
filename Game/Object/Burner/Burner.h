#pragma once

#include "BurnerVertical.h"
#include "BurnerHorizon.h"

/// <summary>
/// 縦横向きのトレイルをまとめたバーナークラス
/// </summary>
class Burner
{
public:
	// コンストラクタ
	Burner();
	// デストラクタ
	~Burner();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime ,DirectX::SimpleMath::Vector3 pos,DirectX::SimpleMath::Quaternion rotate);
	// トレイルをクリア
	void ClearTrail();
	// 親を取得
	GameObject* GetOwner() { return m_owner; }
private:
	// 親
	GameObject* m_owner;
	// 横向きのトレイル
	std::unique_ptr<BurnerVertical> m_vertical;
	// 縦向きのトレイル
	std::unique_ptr<BurnerHorizon>  m_horizon;
};