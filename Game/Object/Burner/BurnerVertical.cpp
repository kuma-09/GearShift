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
}

void BurnerVertical::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	GetComponent<Trail>()->SetPos(GetPosition() - Vector3(0, 0.5f, 0),GetPosition() + Vector3(0, 0.5f, 0));
}
