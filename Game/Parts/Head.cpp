#include "pch.h"
#include "Head.h"
#include "Game/Components/Camera.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include <Game/Components/HP.h>

Head::Head()
{
	AddComponent<HP>();
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
	SetTypeID(TypeID::Head);
	m_isHit = false;
}

Head::~Head()
{
	Finalize();
}

void Head::Initialize(int hp,IScene* scene)
{
	SetScene(scene);
	GetComponent<HP>()->SetHP(hp);
	SetMaxHP(float(hp));
	GetComponent<ModelDraw>()->Initialize(ModelDraw::Head);
}

void Head::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	SetPosition(GetOwner()->GetPosition() 
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Up() * 1.3f);

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);

	ComponentsUpdate(elapsedTime);
	m_isHit = false;
}

void Head::Render(DirectX::SimpleMath::Matrix world)
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

	//GetComponent<BoxCollider>()->Render();

}

void Head::Finalize()
{
}

void Head::Collision(BoxCollider* collider)
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