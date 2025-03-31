#pragma once
#include "Game/GameObject.h"

/// <summary>
/// 横向きのトレイル
/// </summary>
class BurnerVertical : public GameObject
{
public:
	// コンストラクタ
	BurnerVertical();
	// デストラクタ
    ~BurnerVertical();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);


private:
};
