#include "pch.h"
#include "DropItemB.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Physics.h"

#include "Game/Manager/ObjectManager.h"

DropItemB::DropItemB(IScene* scene)
{
	SetScene(scene);
	AddComponent<Physics>();
	AddComponent<Collider>();
	AddComponent<ModelDraw>();

	using namespace DirectX;

	SetScale({ m_dropItemModelSize,m_dropItemModelSize,m_dropItemModelSize });

}

DropItemB::~DropItemB()
{
	Finalize();
}

void DropItemB::Initialize()
{
	using namespace DirectX::SimpleMath;
	GetComponent<Physics>()->Initialize();
	GetComponent<Collider>()->Initialize(Collider::DropItemB,Collider::Collision, { 2,2,2 });
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::DropItem), true);

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
	GetComponent<ModelDraw>()->CreateShadow();
}

void DropItemB::Render()
{
	GetComponent<ModelDraw>()->Render();
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
		ObjectManager::Remove(this);
	}
}