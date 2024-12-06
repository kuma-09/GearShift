#include "pch.h"
#include "Sword.h"
#include "Game/Components/Camera.h"
#include "Game/Components/Collider.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Trail.h"
#include "Game/PlayScene.h"
#include "Framework/Audio.h"

#include "Game/System/HitStop.h"
#include "Framework/Easing.h"

Sword::Sword(IScene* scene, Collider::TypeID id)
{
	SetScene(scene);
	AddComponent<Collider>();
	AddComponent<ModelDraw>();
	AddComponent<Trail>();
	GetComponent<Collider>()->SetTypeID(id);
	GetComponent<Collider>()->SetSize({ 5,4,5 });
	GetComponent<Trail>()->Initialize(L"Resources/Textures/green.png", 10);
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
		HitStop::SetStopTime(0.025f);
	}
}

void Sword::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;


	float rotate = Easing::InQuart(m_rotate, 0.25f);
	if (GetState() == USED)
	{
		m_rotate += elapsedTime;
		if (rotate >= 1)
		{
			m_isHit = true;
		}
	}



	SetPosition(m_owner->GetPosition() + Vector3::Transform({ 0,0,-1 }, m_owner->GetQuaternion()));
	ComponentsUpdate(elapsedTime);
	SetPosition(m_owner->GetPosition());
	SetQuaternion(m_owner->GetQuaternion());

	Matrix world = Matrix::Identity;
	for (int i = 3; i < 5; i++)
	{
		world = Matrix::CreateScale(GetScale());
		world *= Matrix::CreateTranslation(Vector3{ 0,0,i * -1.f });
		if (GetState() == USED)
		{
			world *= Matrix::CreateFromAxisAngle(Vector3(2, 2, 0), XMConvertToRadians(100 - rotate * 200));
		}
		world *= Matrix::CreateFromQuaternion(GetQuaternion());
		world *= Matrix::CreateTranslation(GetPosition());
		static_cast<PlayScene*>(m_owner->GetScene())->CreateHitParticle(world);
	}
	if (!m_isHit)
	{
		GetComponent<Trail>()->SetPos(GetPosition(), { world._41, world._42, world._43 });
	}
	world = Matrix::CreateScale(GetScale());
	if (GetState() == USED)
	{
		world *= Matrix::CreateFromAxisAngle(Vector3(2, 2, 0), XMConvertToRadians(100 - rotate * 200));
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
		GetComponent<Collider>()->Render();
		GetComponent<Trail>()->Render();
	}

}

void Sword::Collision(Collider* collider)
{
	UNREFERENCED_PARAMETER(collider);
}