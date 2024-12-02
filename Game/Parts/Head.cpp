#include "pch.h"
#include "Head.h"
#include "Game/Game.h"
#include "Game/Components/Camera.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include <Game/Components/HP.h>
#include "Game/PlayScene.h"

Head::Head()
{
	AddComponent<HP>();
	AddComponent<ModelDraw>();
	AddComponent<Collider>();
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
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetHeadModel());
	GetComponent<Collider>()->SetSize({ 0.3f,0.3f,0.3f });
}

void Head::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	Vector3 pos{ 0.0f,1.3f,0.0f };
	SetPosition(GetOwner()->GetPosition() + Vector3::Transform(pos, GetOwner()->GetQuaternion()));

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);

	ComponentsUpdate(elapsedTime);
	m_isHit = false;
}

void Head::Render()
{
	if (GetComponent<HP>()->GetHP() > 0)
	{
		if (!m_isHit)
		{
			GetComponent<ModelDraw>()->Render(GetWorld(), false);
		}
		else
		{
			GetComponent<ModelDraw>()->Render(GetWorld(), false, DirectX::Colors::Red);
		}
	}
	else
	{
		//GetComponent<ModelDraw>()->Render(GetWorld(), false, DirectX::Colors::Black);
	}
	//GetComponent<Collider>()->Render();
}

void Head::Finalize()
{
}

void Head::Collision(Collider* collider)
{
	// ÉpÅ[ÉcÇ∆ÇÃìñÇΩÇËîªíË
	if (GetComponent<Collider>()->GetBoundingBox()->Intersects(*collider->GetBoundingBox()))
	{
		Bullet* bulletObject = static_cast<Bullet*>(collider->GetOwner());
		if (bulletObject->GetState() == Bullet::FLYING)
		{
			if (GetComponent<HP>()->GetHP() > 0)
			{
				GetOwner()->GetComponent<Camera>()->shake();
				GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 1);
				bulletObject->Hit();
				static_cast<PlayScene*>(GetOwner()->GetScene())->SetNoise();
				m_isHit = true;
			}
			else
			{
				//auto game = static_cast<PlayScene*>(GetOwner()->GetScene())->GetGame();
				//game->ChangeScene(game->GetGameOverScene());
			}
		}
	}
}