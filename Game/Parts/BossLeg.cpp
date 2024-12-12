#include "pch.h"
#include "BossLeg.h"
#include "Game/Game.h"
#include "Game/Player/Player.h"
#include "Game/Components/Camera.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Move.h"
#include <Game/Components/HP.h>
#include "Game/PlayScene.h"

BossLeg::BossLeg()
{
	AddComponent<HP>();
	AddComponent<ModelDraw>();
	AddComponent<Collider>();
	AddComponent<Emitter>();
	m_isHit = false;
}

BossLeg::~BossLeg()
{
	Finalize();
}

void BossLeg::Initialize(int hp, IScene* scene)
{
	SetScene(scene);
	float scale = 5.0f;
	SetScale({ scale,scale,scale });
	GetComponent<HP>()->SetHP(hp);
	SetMaxHP(float(hp));
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetCutoRobotModel());
	GetComponent<Collider>()->SetSize({ 1.0f,1.0f,1.0f });
	GetComponent<Collider>()->SetInitalePosition({ 0,0,0 });
	GetComponent<Emitter>()->Initialize(L"Resources/Textures/smoke_white.png", 0.1f, 0.1f, 0.3f);
}

void BossLeg::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	Quaternion quaternion = GetOwner()->GetQuaternion();
	Vector3 pos{ 0.0f, -1.0f,0.0f };
	Vector3 velocity = Vector3::Zero;
	if (GetOwner()->GetComponent<Move>())
	{
		velocity = GetOwner()->GetComponent<Move>()->GetVelocity() / 3;
	}
	SetPosition(GetOwner()->GetPosition() + Vector3::Transform(pos, quaternion));
	SetVelocity(GetOwner()->GetVelocity());
	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(-quaternion);
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);

	ComponentsUpdate(elapsedTime);

	m_isHit = false;
}

void BossLeg::Render()
{
	GetComponent<ModelDraw>()->Render();
}

void BossLeg::Finalize()
{
}

void BossLeg::Collision(Collider* collider)
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

void BossLeg::Action()
{
	static_cast<Player*>(GetOwner())->Shot();
}
