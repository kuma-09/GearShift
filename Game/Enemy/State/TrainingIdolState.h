#pragma once
#include "Game/IState.h"

/// <summary>
/// トレーニング用の敵ステート
/// </summary>
class TrainingIdolState : public IState
{
public:
	// コンストラクタ
	TrainingIdolState();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render();
	// 終了処理
	void Finalize();
private:
};
