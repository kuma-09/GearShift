#include "pch.h"
#include "BossEnemy.h"
#include <iostream>
#include <algorithm>
#include "Framework/Audio.h"
#include "Game/Components/HP.h"
#include "Game/Components/Look.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/HPBar.h"
#include "Game/Components/Physics.h"

#include "Game/Object/Sword.h"
#include "Game/PlayScene.h"

#include "Game/Enemy/BossState/BossGatlingState.h"
#include "Game/Enemy/BossState/BossMissileState.h"
#include "Game/Enemy/BossState/BossMoveState.h"

#include "Game/Parts/BossHead.h"
#include "Game/Parts/BossLeg.h"

#include "Game/Manager/ObjectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="scene">シーン</param>
/// <param name="target">ターゲットオブジェクト</param>
BossEnemy::BossEnemy(IScene* scene,GameObject* target)
{
	SetScene(scene);
	SetTarget(target);
	SetScale({ 3.0f,3.0f,3.0f });

	// コンポーネントを追加
	AddComponent<HP>();
	AddComponent<Physics>();
	AddComponent<Collider>();
	AddComponent<HPBar>();
	AddComponent<Look>();

	// 弾を生成
	CreateBullets();
	// ステートを作成
	CreateStates();

}

// デストラクタ
BossEnemy::~BossEnemy()
{

}

// 初期化処理
void BossEnemy::Initialize()
{
	using namespace DirectX::SimpleMath;

	// パーツを生成
	SetPart(Part::Head, std::make_unique<BossHead>(GetTarget()));
	SetPart(Part::BodyTop, std::make_unique<BossLeg>());

	// コンポーネントを初期化
	GetComponent<HP>()->Initialize(25);
	GetComponent<Physics>()->Initialize();
	GetComponent<Look>()->Initialize(false, true);
	GetComponent<Look>()->SetTarget(this, GetTarget());
	GetComponent<Collider>()->Initialize(Collider::Enemy,Collider::Collision, { 6,5,6 });
	GetComponent<HPBar>()->Initialize();

	// 弾を初期化
	for (auto& bullet : m_bullets)
	{
		bullet->Initialize(this);
	}
	for (auto& bullet : m_homingBullets)
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
void BossEnemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	// ステートを更新
	m_state->Update(elapsedTime);

	CheckHP();

	// 弾を更新
	for (auto& bullet : m_bullets)
	{
		bullet->Update(elapsedTime);
	}
	for (auto& bullet : m_homingBullets)
	{
		bullet->Update(elapsedTime);
	}
	
	// コンポーネントを更新
	ComponentsUpdate(elapsedTime);
	// パーツを更新
	UpdateParts(elapsedTime);

	// 座標の移動
	SetPosition(GetPosition() + Vector3::Transform(GetVelocity(), GetQuaternion()));

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition() + Vector3{ 0,-0.9f,0 });

	SetWorld(world);
}

// 描画処理
void BossEnemy::Render()
{
}

// 終了処理
void BossEnemy::Finalize()
{
}

// 弾を発射
void BossEnemy::Shot()
{
	// 使用されていない弾を発射
	for (auto& bullet : m_bullets)
	{
		if (bullet->GetState() == Bullet::BulletState::UNUSED)
		{
			bullet->Shot(static_cast<Player*>(GetTarget()));
			break;
		}
	}
}

// ミサイルを発射
void BossEnemy::ShotMissile()
{
	// 使用されていないミサイルを発射
	for (auto& bullet : m_homingBullets)
	{
		if (bullet->GetState() == Bullet::BulletState::UNUSED)
		{
			bullet->Shot(static_cast<Player*>(GetTarget()));
			break;
		}
	}
}

// ガトリングをリロード
void BossEnemy::ReloadGatling()
{
	for (auto& bullet : m_bullets)
	{
		bullet->Initialize(this);
	}
}

// ステートを変更
void BossEnemy::ChangeState(State* state)
{
	m_state = state;
	m_state->Initialize();
}

// 当たり判定の処理
void BossEnemy::Collision(Collider* collider)
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
		else if(bulletObject->GetState() == Sword::USED)
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


// 弾を作成
void BossEnemy::CreateBullets()
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		m_bullets.push_back(std::make_unique<EnemyBullet>(GetScene(), Collider::TypeID::EnemyBullet));
	}

	for (int i = 0; i < MAX_HOMING_BULLET; i++)
	{
		m_homingBullets.push_back(std::make_unique<HomingBullet>(GetScene(), Collider::TypeID::EnemyBullet));
	}
}

// ステートを作成
void BossEnemy::CreateStates()
{
	SetEnemyAttack(std::make_unique<BossGatlingState>(this));
	SetEnemyMove(std::make_unique<BossMoveState>(this));
	m_missileState = std::make_unique<BossMissileState>(this);
	m_tackleState = std::make_unique<BossTackleState>(this);
	m_state = GetMoveState();
}

// HPが残っているかチェック
void BossEnemy::CheckHP()
{
	if (GetComponent<HP>()->GetHP() <= 0)
	{
		// HPが無ければオブジェクトを削除
		ObjectManager::Remove(this);
		static_cast<PlayScene*>(GetScene())->CreateHitEffect(GetPosition());
		Audio::GetInstance()->PlaySoundSE_Explosion();
	}
}
