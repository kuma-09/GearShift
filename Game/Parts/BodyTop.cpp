#include "pch.h"
#include "BodyTop.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

BodyTop::BodyTop()
{
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
}

BodyTop::~BodyTop()
{
	Finalize();
}


void BodyTop::Initialize(int hp,IScene* scene)
{
	SetScene(scene);
	SetHP(hp);
	GetComponent<ModelDraw>()->Initialize(ModelDraw::BodyTop);
}

void BodyTop::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;


	SetPosition(GetOwner()->GetPosition());/*
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Up() * 0.5f);*/

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);

	ComponentsUpdate(elapsedTime);

	m_isHit = false;
}

void BodyTop::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);

	if (GetHP() > 0)
	{
		if (!m_isHit)
		{
			GetComponent<ModelDraw>()->Render(GetWorld());
		}
		else
		{
			GetComponent<ModelDraw>()->Render(GetWorld(), DirectX::Colors::Red);
		}
	}
	else
	{
		GetComponent<ModelDraw>()->Render(GetWorld(), DirectX::Colors::Black);
	}

}

void BodyTop::Finalize()
{
	RemoveAllComponents();
}

void BodyTop::Collision(BoxCollider* collider)
{
	// ÉpÅ[ÉcÇ∆ÇÃìñÇΩÇËîªíË
	if (GetComponent<BoxCollider>()->GetBoundingBox()->Intersects(*collider->GetBoundingBox()) && GetHP() > 0)
	{
		Bullet* bulletObject = static_cast<Bullet*>(collider->GetOwner());
		if (bulletObject->GetState() == Bullet::FLYING)
		{
			SetHP(GetHP() - 1);
			bulletObject->Hit();
			m_isHit = true;
		}
	}
}