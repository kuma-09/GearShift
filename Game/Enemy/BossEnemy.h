#pragma once
#include "Enemy.h"
#include "Game/Parts/Part.h"

#include "Game/Object/Bullet/HomingBullet.h"
#include "Game/Object/Bullet/EnemyBullet.h"

#include "Game/Enemy/BossState/BossMissileState.h"
#include "Game/Enemy/BossState/BossTackleState.h"

class IState;

/// <summary>
/// ボスエネミー
/// </summary>
class BossEnemy : public Enemy
{
public:

	// コンストラクタ
	BossEnemy(Scene* scene, GameObject* target);
	// デストラクタ
	~BossEnemy();

	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render();
	// 終了処理
	void Finalize();

	// ガトリングを発射
	void Shot();
	// ミサイルを発射
	void ShotMissile();

	// ガトリングをリロード
	void ReloadGatling();
	// ステートを変更
	void ChangeState(IState* state);

	// ミサイル発射ステートを取得
	IState* GetMissileState() { return m_missileState.get(); }
	// 突撃ステートを取得
	IState* GetTackleState() { return m_tackleState.get(); }
	// 当たり判定の処理
	void Collision(Collider* collider);

private:

	void CreateBullets();
	void CreateStates();
	void CheckHP();

	// 弾の最大数
	const int MAX_BULLET = 10;
	const int MAX_HOMING_BULLET = 10;

	// 弾の配列
	std::vector<std::unique_ptr<EnemyBullet>>  m_bullets;
	std::vector<std::unique_ptr<HomingBullet>> m_homingBullets;

	// ステート
	IState* m_state;
	std::unique_ptr<BossMissileState> m_missileState;
	std::unique_ptr<BossTackleState> m_tackleState;
};