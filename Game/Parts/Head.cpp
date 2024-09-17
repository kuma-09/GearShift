#include "pch.h"
#include "Head.h"
#include "Game/Components/Camera.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

Head::Head()
{
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
}

Head::~Head()
{
	Finalize();
}

void Head::Initialize(int hp,IScene* scene)
{
	SetScene(scene);
	SetHP(hp);
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

	//GetComponent<HPBar>().lock().get()->Render(GetPosition() + DirectX::SimpleMath::Vector3{ 0,2,0 });

	//GetComponent<BoxCollider>().lock().get()->Render();

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