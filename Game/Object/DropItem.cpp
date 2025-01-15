#include "pch.h"
#include "DropItem.h"
#include "Game/Components/Physics.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/HP.h"
#include "Game/Manager/ObjectManager.h"

DropItem::DropItem(IScene* scene)
{
	SetScene(scene);
	AddComponent<Physics>();
	AddComponent<Collider>();
	AddComponent<ModelDraw>();

	using namespace DirectX;

	SetScale({ m_dropItemModelSize,m_dropItemModelSize,m_dropItemModelSize });

	// モデルのエフェクト情報を更新する
	Resources::GetInstance()->GetDropItemModel()->UpdateEffects([](IEffect* effect)
		{
			// ベーシックエフェクトを設定する
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				// モデルを自発光させる
				basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);
}

DropItem::~DropItem()
{
	Finalize();
}

void DropItem::Initialize()
{
	using namespace DirectX::SimpleMath;
	GetComponent<Physics>()->Initialize();
	GetComponent<Collider>()->Initialize(Collider::DropItemB,Collider::Collision, { 2,2,2 });
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetDropItemModel(), true);


	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void DropItem::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetQuaternion(GetQuaternion() * Quaternion::CreateFromYawPitchRoll({ 0,1 * 3.14 / 180,0 }));
	SetPosition(GetPosition() + GetVelocity() * elapsedTime);

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void DropItem::CreateShadow()
{
	GetComponent<ModelDraw>()->CreateShadow();
}

void DropItem::Render()
{
	GetComponent<ModelDraw>()->Render();
	if (m_isHit)
	{
		//m_menu->Render();
	}
}

void DropItem::Finalize()
{
}

void DropItem::Collision(Collider* collider)
{
	if (collider->GetTypeID() == Collider::Floor ||
		collider->GetTypeID() == Collider::Wall)
	{
		Collider::CheckHit(this, collider->GetOwner());
	}
	if (collider->GetTypeID() == Collider::Player)
	{
		collider->GetOwner()->GetComponent<HP>()->SetHP(collider->GetOwner()->GetComponent<HP>()->GetHP() + 5);
		ObjectManager::Remove(this);
	}
}
