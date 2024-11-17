#include "pch.h"
#include "LeftArm.h"
#include "Game/Game.h"
#include "Game/Components/Camera.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include <Game/Components/HP.h>
#include "Game/PlayScene.h"

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
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetlArmModel());
	GetComponent<BoxCollider>()->SetSize({ 0.3f,0.5f,0.3f });
	GetComponent<BoxCollider>()->SetInitalePosition({ 0,-0.3f,0 });
}

void LeftArm::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;



	Quaternion quaternion = GetOwner()->GetQuaternion();
	Vector3 pos{ -0.6f, 0.9f, 0.0f };
	SetPosition(GetOwner()->GetPosition() + Vector3::Transform(pos, quaternion));

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(quaternion);
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);

	ComponentsUpdate(elapsedTime);

	m_isHit = false;
}

void LeftArm::Render()
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
		GetComponent<ModelDraw>()->Render(GetWorld(), false, DirectX::Colors::Black);
	}
	//GetComponent<BoxCollider>()->Render();
}

void LeftArm::Finalize()
{
}

void LeftArm::Collision(BoxCollider* collider)
{
	// ÉpÅ[ÉcÇ∆ÇÃìñÇΩÇËîªíË
	if (GetComponent<BoxCollider>()->GetBoundingBox()->Intersects(*collider->GetBoundingBox()))
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
				auto game = static_cast<PlayScene*>(GetOwner()->GetScene())->GetGame();
				game->ChangeScene(game->GetGameOverScene());
			}
		}
	}
}