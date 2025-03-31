#include "pch.h"
#include "LeftArm.h"
#include "Game/Game.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include <Game/Components/HP.h>
#include "Game/PlayScene.h"

LeftArm::LeftArm()
{
	AddComponent<ModelDraw>();
	SetTypeID(TypeID::LeftArm);
	m_isHit = false;
}

LeftArm::~LeftArm()
{
	Finalize();
}

void LeftArm::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::LArm));
	GetComponent<ModelDraw>()->SetRimLithgColor({ 0.75f,0.75f,1,1 });
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

void LeftArm::Finalize()
{
}

void LeftArm::Collision(Collider* collider)
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