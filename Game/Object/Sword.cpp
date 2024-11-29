#include "pch.h"
#include "Sword.h"
#include "Game/Components/Camera.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Trail.h"
#include "Game/PlayScene.h"
#include "Framework/Audio.h"

Sword::Sword(IScene* scene, BoxCollider::TypeID id)
{
	SetScene(scene);
	AddComponent<BoxCollider>();
	AddComponent<ModelDraw>();
	AddComponent<Trail>();
	GetComponent<BoxCollider>()->SetTypeID(BoxCollider::PlayerSword);
	GetComponent<BoxCollider>()->SetSize({ 5,4,5 });
	GetComponent<Trail>()->Initialize(L"Resources/Textures/particle.png", 10);
	SetScale({7.5f, 7.5f, 7.5f});
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

	if (GetState()!=USED)
	{
		Vector3 velocity = Vector3::Zero;
		SetPosition(m_owner->GetPosition());
		SetQuaternion(m_owner->GetQuaternion());
		m_owner->GetComponent<Camera>()->shake();
		SetState(USED);
		Audio::GetInstance()->PlaySoundSE_Slash();
	}
}

void Sword::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;



	if (GetState() == USED)
	{
		m_rotate += elapsedTime * 500;
		if (m_rotate >= 200)
		{
			m_isHit = true;
		}
	}


	SetPosition(m_owner->GetPosition() + Vector3::Transform({0,0,-1},m_owner->GetQuaternion()));
	ComponentsUpdate(elapsedTime);
	SetPosition(m_owner->GetPosition());
	SetQuaternion(m_owner->GetQuaternion());

	Matrix world = Matrix::Identity;
	for (int i = 0; i < 5; i++)
	{
		world = Matrix::CreateScale(GetScale());
		world *= Matrix::CreateTranslation(Vector3{ 0,0,i / 2.f * -1.f});
		if (GetState() == USED)
		{
			world *= Matrix::CreateFromAxisAngle(Vector3(2, 2, 0), XMConvertToRadians(100 - m_rotate));
		}
		world *= Matrix::CreateFromQuaternion(GetQuaternion());
		world *= Matrix::CreateTranslation(GetPosition());
		static_cast<PlayScene*>(m_owner->GetScene())->CreateHitParticle(world);
	}
	GetComponent<Trail>()->SetPos(GetPosition(), GetPosition() + Vector3(0, 1, 0));
	world = Matrix::CreateScale(GetScale());
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
	if (GetState() == USED)
	{
		GetComponent<ModelDraw>()->Render(GetWorld(),false,DirectX::Colors::White);
		GetComponent<BoxCollider>()->Render();
		GetComponent<Trail>()->Render();
	}

}

void Sword::Collision(BoxCollider* collider)
{
	UNREFERENCED_PARAMETER(collider);
}