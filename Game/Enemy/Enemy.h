#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"
#include "Game/GameObject.h"
#include "Game/Player/Player.h"
#include "State/EnemyAttackState.h"
#include "State/EnemyMoveState.h"

class IState;
class EnemyBullet;

/// <summary>
/// 敵の基底クラス
/// </summary>
class Enemy : public GameObject
{
public:
	// デストラクタ
	~Enemy() = default;

	virtual void Initialize() = 0;
	virtual void Update(float elapsedTime) = 0;
	virtual void Render() = 0;
	virtual void Finalize() = 0;

	// 攻撃ステート
	void SetEnemyAttack(std::unique_ptr<IState> state) { m_attack = std::move(state); }
	IState* GetAttackState() { return m_attack.get(); }
	// 移動ステート
	void SetEnemyMove(std::unique_ptr<IState> state) { m_move = std::move(state); }
	IState* GetMoveState() { return m_move.get(); }
	// ターゲット
	void SetTarget(GameObject* target) { m_target = target; }
	GameObject* GetTarget() { return m_target; }

	// 弾発射関数
	virtual void Shot() = 0;
	// ステートの変更
	virtual void ChangeState(IState* state) = 0;
	// 当たり判定の処理
	virtual void Collision(Collider* collider) = 0;

private:
	GameObject* m_target;
	std::unique_ptr<IState> m_attack;
	std::unique_ptr<IState>   m_move;
};

