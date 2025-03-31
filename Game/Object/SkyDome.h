#pragma once
#include "Game/GameObject.h"
#include "Framework/Graphics.h"

/// <summary>
/// 天球クラス
/// </summary>
class SkyDome :public GameObject
{
public:
	// コンストラクタ
	SkyDome();
	// デストラクタ
	~SkyDome();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
private:
};
