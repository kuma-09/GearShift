#pragma once
#include "Enemy.h"

class IState;

/// <summary>
/// トレーニング用のエネミー
/// </summary>
class TrainingEnemy : public Enemy
{
public:
	// コンストラクタ
	TrainingEnemy(Scene* scene);
	// デストラクタ
	~TrainingEnemy();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render();
	// 終了処理
	void Finalize();

	// 弾を発射
	void Shot();
	// ステートを変更
	void ChangeState(IState* state);
	// 当たり判定の処理
	void Collision(Collider* collider);

private:
	// HPが残っているかチェック
	void CheckHP();
};

