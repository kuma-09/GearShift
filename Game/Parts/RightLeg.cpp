#include "pch.h"
#include "RightLeg.h"
#include "Game/Game.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Components/Camera.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Move.h"
#include <Game/Components/HP.h>
#include "Game/PlayScene.h"

RightLeg::RightLeg()
{
	AddComponent<ModelDraw>();
	AddComponent<Emitter>();
	SetTypeID(TypeID::RightLeg);
	m_isHit = false;
}

RightLeg::~RightLeg()
{
	Finalize();
}

void RightLeg::Initialize(int hp,IScene* scene)
{
	UNREFERENCED_PARAMETER(hp);
	SetScene(scene);
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetrLegModel());
	GetComponent<Emitter>()->Initialize(L"Resources/Textures/smoke_white.png", 0.3f, 0.1f, 0.2f);
}

void RightLeg::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;



	Quaternion quaternion = GetOwner()->GetQuaternion();
	Vector3 pos{ 0.4f,-0.2f,-0.0f };
	Vector3 velocity = Vector3::Zero;
	if (GetOwner()->GetComponent<Move>())
	{
		velocity = GetOwner()->GetComponent<Move>()->GetVelocity() / 1.25f;
		if (velocity.x > 0)
		{
			velocity.x /= 2;
		}
	}
	SetPosition(GetOwner()->GetPosition() + Vector3::Transform(pos, quaternion));
	SetVelocity(GetOwner()->GetVelocity());


	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());

	world *= Matrix::CreateTranslation({ 0,-0.5f,0 });
	world *= Matrix::CreateFromYawPitchRoll({ velocity.z ,0,-velocity.x });
	world *= Matrix::CreateTranslation({ 0,0.5f,0 });

	world *= Matrix::CreateFromQuaternion(quaternion);
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);

	ComponentsUpdate(elapsedTime);

	m_isHit = false;
}


void RightLeg::Render()
{
}

void RightLeg::Finalize()
{
}

void RightLeg::Collision(Collider* collider)
{
	UNREFERENCED_PARAMETER(collider);
	//// ÉpÅ[ÉcÇ∆ÇÃìñÇΩÇËîªíË
	//if (GetComponent<Collider>()->GetBoundingBox()->Intersects(*collider->GetBoundingBox()))
	//{
	//	Bullet* bulletObject = static_cast<Bullet*>(collider->GetOwner());
	//	if (bulletObject->GetState() == Bullet::FLYING)
	//	{
	//		if (GetComponent<HP>()->GetHP() > 0)
	//		{
	//			GetOwner()->GetComponent<Camera>()->shake();
	//			GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 1);
	//			bulletObject->Hit();
	//			static_cast<PlayScene*>(GetOwner()->GetScene())->SetNoise();
	//			m_isHit = true;
	//		}
	//		else
	//		{
	//			//auto game = static_cast<PlayScene*>(GetOwner()->GetScene())->GetGame();
	//			//game->ChangeScene(game->GetGameOverScene());
	//		}
	//	}
	//}
}