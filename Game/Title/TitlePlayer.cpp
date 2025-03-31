#include "pch.h"
#include "TitlePlayer.h"

#include "Game/Components/HPBar.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Move.h"
#include "Game/Components/Physics.h"
#include "Game/Components/Look.h"

#include "Game/Parts/Head.h"
#include "Game/Parts/BodyTop.h"
#include "Game/Parts/LeftArm.h"
#include "Game/Parts/RightArm.h"
#include "Game/Parts/LeftLeg.h"
#include "Game/Parts/RightLeg.h"

#include "Game/Shader/ShadowMap.h"

TitlePlayer::TitlePlayer(IScene* scene)
{
	SetScene(scene);
	SetPart(Part::Head, std::make_unique<Head>());
	SetPart(Part::BodyTop, std::make_unique<BodyTop>());
	SetPart(Part::LeftArm, std::make_unique<LeftArm>());
	SetPart(Part::RightArm, std::make_unique<RightArm>());
	SetPart(Part::LeftLeg, std::make_unique<LeftLeg>());
	SetPart(Part::RightLeg, std::make_unique<RightLeg>());
	AddComponent<Move>();
}

TitlePlayer::~TitlePlayer()
{
	Finalize();
}

void TitlePlayer::Initialize()
{
	using namespace DirectX::SimpleMath;
	m_gun = std::make_unique<Gun>(this);
	m_gun->Initialize();
}

void TitlePlayer::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	UpdateParts(elapsedTime);

	m_gun->Update(elapsedTime);
	Matrix world = Matrix::Identity;
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void TitlePlayer::Finalize()
{
}

void TitlePlayer::Collision(Collider* collider)
{
	UNREFERENCED_PARAMETER(collider);
}

