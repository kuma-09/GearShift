#include "pch.h"
#include "SkyDome.h"
#include "Framework/Resources.h"
#include "Game/Components/ModelDraw.h"

SkyDome::SkyDome()
{
	using namespace DirectX;
	m_graphics = Graphics::GetInstance();
	SetWorld(SimpleMath::Matrix::Identity);
	AddComponent<ModelDraw>();
}

SkyDome::~SkyDome()
{

}

void SkyDome::Initialize(DirectX::SimpleMath::Vector3 pos)
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetSkyDome(), true);
	auto world = DirectX::SimpleMath::Matrix::CreateScale(1.0f);
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(pos);
	SetWorld(world);
}

void SkyDome::Update(float elapsedTime)
{
	ComponentsUpdate(elapsedTime);
}

void SkyDome::Render()
{
}