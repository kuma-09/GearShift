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

void SkyDome::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::SkyDome), true);
	GetComponent<ModelDraw>()->SetEmissiveColor({0.25f,0.25f,0.25f,1.0f});
}

void SkyDome::Update(float elapsedTime)
{
	ComponentsUpdate(elapsedTime);
}

void SkyDome::Render()
{
}