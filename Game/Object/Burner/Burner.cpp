#include "pch.h"	
#include "Burner.h"

Burner::Burner()
{
	m_vertical = std::make_unique<BurnerVertical>();
	m_horizon = std::make_unique<BurnerHorizon>();
}

Burner::~Burner()
{
}

void Burner::Initialize()
{
	m_vertical->Initialize();
	m_horizon->Initialize();
}

void Burner::Update(float elapsedTime, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion rotate)
{
	m_vertical->SetPosition(pos);
	m_horizon->SetPosition(pos);

	m_vertical->SetQuaternion(rotate);
	m_horizon->SetQuaternion(rotate);

	m_vertical->Update(elapsedTime);
	m_horizon->Update(elapsedTime);
}
