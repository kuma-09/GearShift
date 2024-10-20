#include "pch.h"
#include "TitlePlayer.h"

#include "Game/Components/HPBar.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/Move.h"
#include "Game/Components/Gravity.h"
#include "Game/Components/Camera.h"
#include "Game/Components/Look.h"

#include "Game/Parts/Head.h"
#include "Game/Parts/BodyTop.h"
#include "Game/Parts/LeftArm.h"
#include "Game/Parts/RightArm.h"
#include "Game/Parts/LeftLeg.h"
#include "Game/Parts/RightLeg.h"



TitlePlayer::TitlePlayer(IScene* scene)
{
	SetScene(scene);
	SetPart(Part::Head, std::make_unique<Head>());
	SetPart(Part::BodyTop, std::make_unique<BodyTop>());
	SetPart(Part::LeftArm, std::make_unique<LeftArm>());
	SetPart(Part::RightArm, std::make_unique<RightArm>());
	SetPart(Part::LeftLeg, std::make_unique<LeftLeg>());
	SetPart(Part::RightLeg, std::make_unique<RightLeg>());
}

TitlePlayer::~TitlePlayer()
{
	Finalize();
}

void TitlePlayer::Initialize()
{
	using namespace DirectX::SimpleMath;
	SetPosition({0,2.3f,0});
}

void TitlePlayer::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	UpdateParts(elapsedTime);

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);

}

void TitlePlayer::CreateShadow()
{
	for (auto& pair : m_pPart)
	{
		pair.second->CreateShadow();
	}
}

void TitlePlayer::Render()
{
	RenderParts();
}

void TitlePlayer::Finalize()
{
}


void TitlePlayer::Collision(BoxCollider* collider)
{
	UNREFERENCED_PARAMETER(collider);
}

