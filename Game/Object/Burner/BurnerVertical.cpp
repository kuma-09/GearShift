#include "pch.h"
#include "BurnerVertical.h"
#include "Game/Components/Trail.h"

BurnerVertical::BurnerVertical()
{
	AddComponent<Trail>();
}

BurnerVertical::~BurnerVertical()
{
}

void BurnerVertical::Initialize()
{
	GetComponent<Trail>()->Initialize(L"Resources/Textures/particle.png", 10, DirectX::Colors::CornflowerBlue);
}

void BurnerVertical::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	GetComponent<Trail>()->SetPos(GetPosition() - Vector3::Transform(Vector3(0.5f,0, 0),GetQuaternion()), GetPosition() + Vector3::Transform(Vector3(0.5f,0, 0), GetQuaternion()));
}
