#include "pch.h"
#include "RightArm.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

RightArm::RightArm()
{
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
}

RightArm::~RightArm()
{

}

void RightArm::Initialize(int hp,IScene* scene)
{
	SetScene(scene);
	SetHP(hp);
	GetComponent<ModelDraw>().lock().get()->Initialize(ModelDraw::RArm);
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::Player);
}

void RightArm::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetPosition(GetOwner()->GetPosition() 
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Right() *  0.6f
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Up()    *  0.9f);

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
	m_isHit = false;
}

void RightArm::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);



	if (GetHP() >= 0)
	{
		if (!m_isHit)
		{
			GetComponent<ModelDraw>().lock().get()->Render(GetWorld());
		}
		else
		{
			GetComponent<ModelDraw>().lock().get()->Render(GetWorld(), DirectX::Colors::Red);
		}
	}
	else
	{
		GetComponent<ModelDraw>().lock().get()->Render(GetWorld(), DirectX::Colors::Black);
	}

	//GetComponent<HPBar>().lock().get()->Render(GetPosition() + DirectX::SimpleMath::Vector3{ 0,2,0 });

	//GetComponent<BoxCollider>().lock().get()->Render();

}

void RightArm::Finalize()
{

}

void RightArm::Collision(BoxCollider* collider)
{
	if (collider->GetTypeID() == BoxCollider::EnemyBullet)
	{
		SetHP(GetHP() - 1);
		m_isHit = true;
	}
}