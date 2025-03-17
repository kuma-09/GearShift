#include "pch.h"
#include "Head.h"
#include "Game/Game.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include <Game/Components/HP.h>
#include "Game/PlayScene.h"

Head::Head()
{
	AddComponent<ModelDraw>();
	SetTypeID(TypeID::Head);
	m_isHit = false;
}

Head::~Head()
{
	Finalize();
}

void Head::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::Head));
	GetComponent<ModelDraw>()->SetRimLithgColor({ 0.75f,0.75f,1,1 });
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
	GetComponent<ModelDraw>()->Render();

}

void Head::Finalize()
{
}

void Head::Collision(Collider* collider)
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