#include "pch.h"
#include "Sword.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/ModelDraw.h"
#include "Game/PlayScene.h"

Sword::Sword(IScene* scene , BoxCollider::TypeID id)
{
	SetScene(scene);
	AddComponent<BoxCollider>();
	AddComponent<ModelDraw>();
	GetComponent<BoxCollider>()->SetTypeID(BoxCollider::PlayerSword);
	GetComponent<BoxCollider>()->SetSize({5,4,5});
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
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetSwordModel());

	Vector3 velocity = Vector3::Zero;
	SetPosition(m_owner->GetPosition());
	SetQuaternion(m_owner->GetQuaternion());

	SetVelocity(Vector3::Zero);
	SetState(SwordState::USING);
	m_rotate = 0;
	m_isHit = false;
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
	SetPosition(m_owner->GetPosition());
	SetQuaternion(m_owner->GetQuaternion());
	SetState(USED);
}

void Sword::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;



	if (GetState() == USED)
	{
		m_rotate += elapsedTime * 1000;
		if (m_rotate >= 200)
		{
			m_isHit = true;
		}
	}


	SetPosition(m_owner->GetPosition() + Vector3::Transform({0,0,-4},m_owner->GetQuaternion()));
	ComponentsUpdate(elapsedTime);
	SetPosition(m_owner->GetPosition());
	SetQuaternion(m_owner->GetQuaternion());
	Matrix world = Matrix::CreateScale(GetScale());
	if (GetState() == USED)
	{
		world *= Matrix::CreateFromAxisAngle(Vector3(2, 2, 0), XMConvertToRadians(100 - m_rotate));
	}
	else
	{
		world *= Matrix::CreateFromQuaternion(Quaternion::CreateFromYawPitchRoll(XMConvertToRadians(100), XMConvertToRadians(15), 0));
	}
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void Sword::Render()
{	
	if (!m_isHit)
	{
		static_cast<PlayScene*>(m_owner->GetScene())->CreateHitParticle(GetWorld());
		GetComponent<ModelDraw>()->Render(GetWorld(),false,DirectX::Colors::LightBlue);
		GetComponent<BoxCollider>()->Render();
	}

}

void Sword::Collision(BoxCollider* collider)
{

}