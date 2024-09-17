#include "pch.h"
#include "BodyBottom.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/PlayScene.h"
#include "Game/Components/Camera.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/HPBar.h"

BodyBottom::BodyBottom()
{
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
	SetTypeID(Part::BodyBottom);
	//AddComponent<HPBar>();
}

BodyBottom::~BodyBottom()
{
	Finalize();
}

void BodyBottom::Initialize(int hp,IScene* scene)
{
	SetScene(scene);
	SetHP(hp);
	GetComponent<ModelDraw>()->Initialize(ModelDraw::BodyBottom);
	//GetComponent<HPBar>().lock().get()->Initialize();
}

void BodyBottom::Update(float elapsedTime)
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

void BodyBottom::Render(DirectX::SimpleMath::Matrix world)
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
			GetComponent<ModelDraw>()->Render(GetWorld(),DirectX::Colors::Red);
		}
	}
	else
	{
		GetComponent<ModelDraw>()->Render(GetWorld(),DirectX::Colors::Black);
	}

	//GetComponent<HPBar>().lock().get()->Render(GetPosition() + DirectX::SimpleMath::Vector3{ 0,2,0 });

	//GetComponent<BoxCollider>().lock().get()->Render();

}

void BodyBottom::Finalize()
{
}

void BodyBottom::Collision(BoxCollider* collider)
{
	// ÉpÅ[ÉcÇ∆ÇÃìñÇΩÇËîªíË
	if (GetComponent<BoxCollider>()->GetBoundingBox()->Intersects(*collider->GetBoundingBox()) && GetHP() > 0)
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