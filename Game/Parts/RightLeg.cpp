#include "pch.h"
#include "RightLeg.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Components/Camera.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/Move.h"
#include <Game/Components/HP.h>

RightLeg::RightLeg()
{
	AddComponent<HP>();
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
	SetTypeID(TypeID::RightLeg);
	m_isHit = false;
}

RightLeg::~RightLeg()
{
	Finalize();
}

void RightLeg::Initialize(int hp,IScene* scene)
{
	SetScene(scene);
	GetComponent<HP>()->SetHP(hp);
	SetMaxHP(float(hp));
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetrLegModel());
}

void RightLeg::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	Quaternion quaternion = GetOwner()->GetQuaternion();
	Vector3 pos{ 0.4f,-0.2f,-0.0f };
	Vector3 velocity = GetOwner()->GetComponent<Move>()->GetVelocity() / 3;
	//velocity = Vector3::Transform(velocity, quaternion);
	SetPosition(GetOwner()->GetPosition() + Vector3::Transform(pos, quaternion));


	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());

	world *= Matrix::CreateTranslation({ 0,-0.5f,0 });
	world *= Matrix::CreateFromYawPitchRoll({ velocity.z ,0,-velocity.x });
	world *= Matrix::CreateTranslation({ 0,0.5f,0 });

	world *= Matrix::CreateFromQuaternion(quaternion);
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
	m_isHit = false;
}

void RightLeg::Render()
{
	if (GetComponent<HP>()->GetHP() > 0)
	{
		if (!m_isHit)
		{
			GetComponent<ModelDraw>()->Render(false);
		}
		else
		{
			GetComponent<ModelDraw>()->Render(false, DirectX::Colors::Red);
		}
	}
	else
	{
		GetComponent<ModelDraw>()->Render(false, DirectX::Colors::Black);
	}

}

void RightLeg::Finalize()
{
}

void RightLeg::Collision(BoxCollider* collider)
{
	// ÉpÅ[ÉcÇ∆ÇÃìñÇΩÇËîªíË
	if (GetComponent<BoxCollider>()->GetBoundingBox()->Intersects(*collider->GetBoundingBox()))
	{
		Bullet* bulletObject = static_cast<Bullet*>(collider->GetOwner());
		if (bulletObject->GetState() == Bullet::FLYING)
		{
			GetOwner()->GetComponent<Camera>()->shake();
			GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 1);
			bulletObject->Hit();
			m_isHit = true;
		}
	}
}