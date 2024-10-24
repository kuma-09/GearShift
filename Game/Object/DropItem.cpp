#include "pch.h"
#include "DropItem.h"
#include "Game/Components/Gravity.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

DropItem::DropItem(IScene* scene, std::unique_ptr<Part> part)
{

	SetScene(scene);
	AddComponent<Gravity>();
	AddComponent<BoxCollider>();
	GetComponent<BoxCollider>()->SetTypeID(BoxCollider::TypeID::DropItem);
	GetComponent<BoxCollider>()->SetSize(DirectX::SimpleMath::Vector3::One * 2);
	AddComponent<ModelDraw>();
	GetComponent<ModelDraw>()->Initialize(ModelDraw::DropItem);

	m_part = std::move(part);

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
	GetComponent<ModelDraw>()->Render(true);
	if (m_isHit)
	{
		//m_menu->Render();
	}
}

void DropItem::Finalize()
{
}

void DropItem::Collision(BoxCollider* collider)
{
	if (collider->GetTypeID() == BoxCollider::Floor ||
		collider->GetTypeID() == BoxCollider::Wall)
	{
		BoxCollider::CheckHit(this, collider->GetOwner());
	}
}
