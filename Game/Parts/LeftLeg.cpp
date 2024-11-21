#include "pch.h"
#include "LeftLeg.h"
#include "Game/Game.h"
#include "Game/Player/Player.h"
#include "Game/Components/Camera.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/Move.h"
#include <Game/Components/HP.h>
#include "Game/PlayScene.h"

LeftLeg::LeftLeg()
{
	AddComponent<HP>();
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
	AddComponent<Emitter>();
	SetTypeID(Part::LeftLeg);
	m_isHit = false;
}

LeftLeg::~LeftLeg()
{
	Finalize();
}

void LeftLeg::Initialize(int hp,IScene* scene)
{
	SetScene(scene);
	GetComponent<HP>()->SetHP(hp);
	SetMaxHP(float(hp));
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetlLegModel());
	GetComponent<BoxCollider>()->SetSize({ 0.3f,0.8f,0.3f });
	GetComponent<BoxCollider>()->SetInitalePosition({ 0,-0.4f,0 });
	GetComponent<Emitter>()->Initialize(L"Resources/Textures/smoke_white.png", 0.1f, 0.1f, 0.3f);
}

void LeftLeg::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	Quaternion quaternion = GetOwner()->GetQuaternion();
	Vector3 velocity = GetOwner()->GetComponent<Move>()->GetVelocity() / 3;
	//velocity = Vector3::Transform(velocity, quaternion);
	Vector3 pos{ -0.4f,-0.2f,-0.0f };
	SetPosition(GetOwner()->GetPosition() + Vector3::Transform(pos, quaternion));
	SetVelocity(GetOwner()->GetVelocity());
	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());

	world *= Matrix::CreateTranslation({ 0,-0.5f,0 });
	world *= Matrix::CreateFromYawPitchRoll({velocity.z,0,-velocity.x});
	world *= Matrix::CreateTranslation({ 0,0.5f,0 });

	world *= Matrix::CreateFromQuaternion(quaternion);
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);

	ComponentsUpdate(elapsedTime);

	m_isHit = false;
}

void LeftLeg::Render()
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
	if (static_cast<Player*>(GetOwner())->GetOnFloor())
	{
		GetComponent<Emitter>()->Render(GetPosition() - DirectX::SimpleMath::Vector3{ 0,1.f,0 });
	}
	//GetComponent<BoxCollider>()->Render();
}

void LeftLeg::Finalize()
{
}

void LeftLeg::Collision(BoxCollider* collider)
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
				//auto game = static_cast<PlayScene*>(GetOwner()->GetScene())->GetGame();
				//game->ChangeScene(game->GetGameOverScene());
			}
		}
	}
}

void LeftLeg::Action()
{
	static_cast<Player*>(GetOwner())->Shot();
}
