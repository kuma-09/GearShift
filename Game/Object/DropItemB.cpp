#include "pch.h"
#include "DropItemB.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Physics.h"

DropItemB::DropItemB(IScene* scene, std::vector<std::unique_ptr<Bullet>> wepons)
{
	SetScene(scene);
	AddComponent<Physics>();
	AddComponent<Collider>();
	GetComponent<Collider>()->SetTypeID(Collider::TypeID::DropItem);
	GetComponent<Collider>()->SetSize(DirectX::SimpleMath::Vector3::One * 2);
	AddComponent<ModelDraw>();
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetDropItemModel());

	m_bullet = std::move(wepons);

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

DropItemB::~DropItemB()
{
	Finalize();
}

void DropItemB::Initialize()
{
	using namespace DirectX::SimpleMath;


	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void DropItemB::Update(float elapsedTime)
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

void DropItemB::CreateShadow()
{
	GetComponent<ModelDraw>()->CreateShadow(GetWorld());
}

void DropItemB::Render()
{
	GetComponent<ModelDraw>()->Render(GetWorld(), true, DirectX::Colors::Red);
}

void DropItemB::Finalize()
{
}

void DropItemB::Collision(Collider* collider)
{
	if (collider->GetTypeID() == Collider::Floor ||
		collider->GetTypeID() == Collider::Wall)
	{
		Collider::CheckHit(this, collider->GetOwner());
	}
	if (collider->GetTypeID() == Collider::Player)
	{

	}
}