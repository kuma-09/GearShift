#include "pch.h"
#include "RightLeg.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Components/Camera.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

RightLeg::RightLeg()
{
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
	SetTypeID(TypeID::RightLeg);
}

RightLeg::~RightLeg()
{
	Finalize();
}

void RightLeg::Initialize(int hp,IScene* scene)
{
	SetScene(scene);
	SetHP(float(hp));
	SetMaxHP(float(hp));
	GetComponent<ModelDraw>()->Initialize(ModelDraw::RLeg);
}

void RightLeg::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetPosition(GetOwner()->GetPosition() 
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Right() * 0.4f 
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Down()  * 0.1f);



	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
	m_isHit = false;
}

void RightLeg::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);



	if (GetHP() >= 0)
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
			SetHP(GetHP() - 1);
			if (GetHP() <= 0)
			{
				GetOwner()->SetHP(GetOwner()->GetHP() - 1);
			}
			bulletObject->Hit();
			m_isHit = true;
		}
	}
}