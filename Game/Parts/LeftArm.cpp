#include "pch.h"
#include "LeftArm.h"
#include "Game/Components/Camera.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include <Game/Components/HP.h>

LeftArm::LeftArm()
{
	AddComponent<HP>();
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
	SetTypeID(TypeID::LeftArm);
	m_isHit = false;
}

LeftArm::~LeftArm()
{
	Finalize();
}

void LeftArm::Initialize(int hp,IScene* scene)
{
	SetScene(scene);
	GetComponent<HP>()->SetHP(hp);
	SetMaxHP(float(hp));
	GetComponent<ModelDraw>()->Initialize(ModelDraw::LArm);
}

void LeftArm::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	SetPosition(GetOwner()->GetPosition() 
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Right() * -0.6f
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Up()    *  0.9f);

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);

	ComponentsUpdate(elapsedTime);
	m_isHit = false;
}

void LeftArm::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);



	if (GetComponent<HP>()->GetHP() >= 0)
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

	//GetComponent<HPBar>().lock().get()->Render(GetPosition() + DirectX::SimpleMath::Vector3{ 0,2,0 });

	//GetComponent<BoxCollider>().lock().get()->Render();

}

void LeftArm::Finalize()
{
}

void LeftArm::Collision(BoxCollider* collider)
{
	// �p�[�c�Ƃ̓����蔻��
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