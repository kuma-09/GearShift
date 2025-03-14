#include "pch.h"
#include "HomingEnemy.h"
#include <iostream>
#include <algorithm>
#include "Framework/Audio.h"
#include "Game/Components/HP.h"
#include "Game/Components/Look.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/HPBar.h"
#include "Game/Object/Bullet/HomingBullet.h"
#include "Game/Object/Sword.h"
#include "Game/PlayScene.h"
#include "Game/Enemy/State/EnemyAttackState.h"
#include "Game/Enemy/State/EnemyMoveState.h"

#include "Game/Manager/ObjectManager.h"

// コンストラクタ
HomingEnemy::HomingEnemy(IScene* scene,GameObject* target)
{
	SetScene(scene);
	SetTarget(target);
	SetScale({ 0.05f,0.05f,0.05f });

	// コンポーネントを追加
	AddComponent<HP>();
	AddComponent<Look>();
	AddComponent<ModelDraw>();
	AddComponent<Collider>();
	AddComponent<HPBar>();

	// 弾を生成
	for (int i = 0; i < BULLET_COUNT; i++)
	{
		m_bullet.emplace_back(std::make_unique<HomingBullet>(GetScene(), Collider::TypeID::EnemyBullet));
	}

	// ステートを生成
	SetEnemyAttack(std::make_unique<EnemyAttackState>(this));
	SetEnemyMove(std::make_unique<EnemyMoveState>(this));
	m_state = GetMoveState();
}

// デストラクタ
HomingEnemy::~HomingEnemy()
{
}

// 初期化処理
void HomingEnemy::Initialize()
{
	using namespace DirectX::SimpleMath;

	// コンポーネントを初期化
	GetComponent<HP>()->SetHP(10);
	GetComponent<Look>()->SetTarget(this, GetTarget());
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::HomingEnemy), true);
	GetComponent<Collider>()->Initialize(Collider::Enemy,Collider::Collision);
	GetComponent<HPBar>()->Initialize();

	// 弾を初期化
	for (auto& bullet : m_bullet)
	{
		bullet->Initialize(this);
	}

	// ステートを初期化
	m_state->Initialize();
	// 座標を初期化
	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

// 更新処理
void HomingEnemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// コンポーネントを更新
	ComponentsUpdate(elapsedTime);
	// ステートを更新
	m_state->Update(elapsedTime);
	// 弾を更新
	for (auto& bullet : m_bullet)
	{
		bullet->Update(elapsedTime);
	}

	// 座標の移動
	SetPosition(GetPosition() + Vector3::Transform(GetVelocity(), GetQuaternion()));
	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);

	// HPが残っているかチェック
	CheckHP();

}

// 描画処理
void HomingEnemy::Render()
{
}

// 終了処理
void HomingEnemy::Finalize()
{
}

// 弾を発射
void HomingEnemy::Shot()
{
	for (auto& bullet : m_bullet)
	{
		if (bullet->GetState() == Bullet::BulletState::UNUSED)
		{
			bullet->Shot(static_cast<Player*>(GetTarget()));
			break;
		}
	}
}

// ステートを変更
void HomingEnemy::ChangeState(State* state)
{
	m_state = state;
	m_state->Initialize();
}

// 当たり判定の処理
void HomingEnemy::Collision(Collider* collider)
{
	// プレイヤーの弾が当たった時の処理
	if (collider->GetTypeID() == Collider::PlayerBullet)
	{
		Bullet* bulletObject = static_cast<Bullet*>(collider->GetOwner());
		if (bulletObject->GetState() == Bullet::FLYING)
		{
			GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 1);
			static_cast<PlayScene*>(GetScene())->CreateHitParticle(bulletObject->GetPosition());
			bulletObject->Hit();
		}
	}
	// プレイヤーのブレードが当たった時の処理
	if (collider->GetTypeID() == Collider::PlayerSword)
	{
		Sword* bulletObject = static_cast<Sword*>(collider->GetOwner());
		if (bulletObject->GetState() == Sword::USING)
		{
			GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 5);
			static_cast<PlayScene*>(GetScene())->CreateHitParticle(bulletObject->GetPosition());
			bulletObject->Hit();
		}
		else if (bulletObject->GetState() == Sword::USED)
		{
			bulletObject->Hit();
		}
	}
	// 床や壁に当たった時の処理
	if (collider->GetTypeID() == Collider::Floor ||
		collider->GetTypeID() == Collider::Wall)
	{
		Collider::CheckHit(this, collider->GetOwner());
	}
}

// HPが残っているかチェック
void HomingEnemy::CheckHP()
{
	if (GetComponent<HP>()->GetHP() <= 0)
	{
		// HPが無ければオブジェクトを削除
		ObjectManager::Remove(this);
		static_cast<PlayScene*>(GetScene())->CreateHitEffect(GetPosition());
		Audio::GetInstance()->PlaySoundSE_Explosion();
	}
}
