#include "pch.h"
#include "BodyTop.h"
#include "Game/Game.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include <Game/Components/HP.h>
#include "Game/PlayScene.h"

BodyTop::BodyTop()
{
	AddComponent<ModelDraw>();
	SetTypeID(TypeID::BodyTop);
	m_isHit = false;
}

BodyTop::~BodyTop()
{
	Finalize();
}


void BodyTop::Initialize(int hp,IScene* scene)
{
	UNREFERENCED_PARAMETER(hp);
	SetScene(scene);
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetBodyTopModel());
	
}

void BodyTop::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;


	Vector3 pos{ 0.0f,0.0f,0.0f };
	SetPosition(GetOwner()->GetPosition() + Vector3::Transform(pos, GetOwner()->GetQuaternion()));

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);

	ComponentsUpdate(elapsedTime);

	m_isHit = false;
}

void BodyTop::Render()
{

	GetComponent<ModelDraw>()->Render();

}

void BodyTop::Finalize()
{
}

void BodyTop::Collision(Collider* collider)
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
	//			if (GetComponent<HP>()->GetHP() <= 0)
	//			{
	//				auto game = static_cast<PlayScene*>(GetOwner()->GetScene())->GetGame();
	//				game->ChangeScene(game->GetGameOverScene());
	//			}
	//		}
	//	}
	//}
}