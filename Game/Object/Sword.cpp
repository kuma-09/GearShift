#include "pch.h"
#include "Sword.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/ModelDraw.h"

Sword::Sword(IScene* scene , BoxCollider::TypeID id)
{
	SetScene(scene);
	AddComponent<BoxCollider>();
	AddComponent<ModelDraw>();
	GetComponent<BoxCollider>()->SetTypeID(id);
	GetComponent<BoxCollider>()->SetSize(m_size);
	SetScale(m_size);
	SetState(SwordState::UNUSED);

}

Sword::~Sword()
{
}

void Sword::Initalize(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	m_owner = object;
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetCubeModel());

	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);

	SetVelocity(Vector3::Zero);
	SetState(SwordState::USING);
}

void Sword::Shot(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	Vector3 velocity = Vector3::Zero;
	SetPosition(object->GetPosition());
	SetQuaternion(object->GetQuaternion());

	velocity += Vector3::Transform(Vector3::Forward, GetQuaternion());

	SetVelocity(velocity);
	SetState(SwordState::USING);
}

void Sword::Hit()
{
	using namespace DirectX::SimpleMath;

	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);

	SetVelocity(Vector3::Zero);
	SetState(SwordState::USED);
}

void Sword::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetPosition(m_owner->GetPosition());

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void Sword::Render()
{	
	if (m_state == USING)
	{
		GetComponent<ModelDraw>()->Render(GetWorld());
	}

}

void Sword::Collision(BoxCollider* collider)
{
	UNREFERENCED_PARAMETER(collider);
}