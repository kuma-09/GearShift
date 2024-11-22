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
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetSwordModel());

	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);

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
	SetPosition(Vector3::Zero);
	//SetQuaternion(Quaternion::Identity);

	SetVelocity(Vector3::Zero);
}

void Sword::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	if (m_isHit)
	{
		m_rotate += elapsedTime * 1500;
		if (m_rotate >= 200)
		{
			SetState(USED);
		}
	}


	SetPosition(m_owner->GetPosition());
	SetQuaternion(m_owner->GetQuaternion());
	Matrix world = Matrix::CreateScale(GetScale());
	if (m_isHit)
	{
		world *= Matrix::CreateFromAxisAngle(Vector3(-2, -2, 0), 100 - m_rotate);
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
	if (m_state != USED)
	{



		static_cast<PlayScene*>(m_owner->GetScene())->CreateHitParticle(GetWorld());
		GetComponent<ModelDraw>()->Render(GetWorld());
		GetComponent<BoxCollider>()->Render();
	}

}

void Sword::Collision(BoxCollider* collider)
{
	if (GetState() == USING)
	{
		if (collider->GetTypeID() == BoxCollider::Enemy)
		{
			Hit();
			m_isHit = true;
		}
	}
}