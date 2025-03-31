#include "pch.h"
#include "FixedEnemy.h"
#include <iostream>
#include <algorithm>
#include <Framework/Audio.h>
#include "Game/Components/HP.h"
#include "Game/Components/Look.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/HPBar.h"
#include "Game/Components/Physics.h"
#include "Game/Object/Bullet/FixedEnemyBullet.h"
#include "Game/Object/Sword.h"
#include "Game/PlayScene.h"
#include "Game/Enemy/State/FixedAttackState.h"
#include "Game/Manager/ObjectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="scene">シーン</param>
/// <param name="target">ターゲットオブジェクト</param>
FixedEnemy::FixedEnemy(IScene* scene,GameObject* target)
{

	SetScene(scene);
	SetTarget(target);
	SetScale({ 3.0f,3.0f,3.0f });

	// コンポーネントを追加
	AddComponent<HP>();
	AddComponent<Look>();
	AddComponent<Physics>();
	AddComponent<ModelDraw>();
	AddComponent<Collider>();
	AddComponent<HPBar>();

	// 弾を生成
	m_bullet = std::make_unique<FixedEnemyBullet>(GetScene(), Collider::TypeID::EnemyBullet);
	
	// ステートを生成
	SetEnemyAttack(std::make_unique<FixedAttackState>(this));
	m_state = GetAttackState();
}

// デストラクタ
FixedEnemy::~FixedEnemy()
{
}

// 初期化処理
void FixedEnemy::Initialize()
{
	using namespace DirectX::SimpleMath;

	// コンポーネントを初期化
	GetComponent<HP>()->SetHP(10);
	GetComponent<Look>()->Initialize(false,false);
	GetComponent<Look>()->SetTarget(this, GetTarget());
	GetComponent<Physics>()->Initialize();
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::TankBody));
	GetComponent<ModelDraw>()->SetRimLithgColor(DirectX::Colors::IndianRed);
	GetComponent<Collider>()->Initialize(Collider::Enemy,Collider::Collision, { 2,0.5f,3 });
	GetComponent<HPBar>()->Initialize();
	// 弾を初期化
	m_bullet->Initialize(this);
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
void FixedEnemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	// ステートを更新
	m_state->Update(elapsedTime);
	// 弾を更新
	m_bullet->Update(elapsedTime);
	// コンポーネントを更新
	ComponentsUpdate(elapsedTime);

	// 座標の移動
	SetPosition(GetPosition() + GetVelocity());
	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
	// HPが残っているかチェック
	CheckHP();
}

// 描画処理
void FixedEnemy::Render()
{	
}

// 終了処理
void FixedEnemy::Finalize()
{
}

// 弾を発射
void FixedEnemy::Shot()
{
	m_bullet->Shot(static_cast<Player*>(GetTarget()));
}

// ステートを変更
void FixedEnemy::ChangeState(State* state)
{
	m_state = state;
	m_state->Initialize();
}

// 当たり判定の処理
void FixedEnemy::Collision(Collider* collider)
{
	// プレイヤーの弾が当たった時の処理
	if (collider->GetTypeID() == Collider::PlayerBullet)
	{
		Bullet* bulletObject = static_cast<Bullet*>(collider->GetOwner());
		if (bulletObject->GetState() == Bullet::FLYING)
		{
			GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 1);
			static_cast<PlayScene*>(GetScene())->CreateHitParticle(GetPosition());
			bulletObject->Hit();
		}
	}
	// プレイヤーのブレードが当たった時の処理
	if (collider->GetTypeID() == Collider::PlayerSword)
	{
		Blade* bulletObject = static_cast<Blade*>(collider->GetOwner());
		if (bulletObject->GetState() == Blade::USING)
		{
			GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 5);
			static_cast<PlayScene*>(GetScene())->CreateHitParticle(GetPosition());
			bulletObject->Hit();
		}
		else if (bulletObject->GetState() == Blade::USED)
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
void FixedEnemy::CheckHP()
{
	if (GetComponent<HP>()->GetHP() <= 0)
	{
		// HPが無ければオブジェクトを削除
		ObjectManager::Remove(this);
		static_cast<PlayScene*>(GetScene())->CreateHitEffect(GetPosition());
		Audio::GetInstance()->PlaySoundSE_Explosion();
	}
}
