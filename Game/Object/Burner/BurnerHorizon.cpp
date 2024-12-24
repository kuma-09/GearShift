#include "pch.h"
#include "BurnerHorizon.h"
#include "Game/Components/Trail.h"

BurnerHorizon::BurnerHorizon()
{
	AddComponent<Trail>();
}

BurnerHorizon::~BurnerHorizon()
{
}

void BurnerHorizon::Initialize()
{
	GetComponent<Trail>()->Initialize(L"Resources/Textures/particle.png", 10, DirectX::Colors::LightBlue);
}

void BurnerHorizon::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	GetComponent<Trail>()->SetPos(GetPosition() - Vector3(0,0.5f, 0), GetPosition() + Vector3(0,0.5f, 0));
}
