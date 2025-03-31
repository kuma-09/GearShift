#pragma once
#include "Game/GameObject.h"

// 縦向きのトレイル
class BurnerHorizon : public GameObject
{
public:
	// コンストラクタ
	BurnerHorizon();
	// デストラクタ
	~BurnerHorizon();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);

private:
};