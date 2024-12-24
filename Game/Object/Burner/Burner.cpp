#include "pch.h"	
#include "Burner.h"

Burner::Burner()
{
	m_vertical = std::make_unique<BurnerVertical>();
}

Burner::~Burner()
{
}

void Burner::Initialize()
{
	m_vertical->Initialize();
}

void Burner::Update(DirectX::SimpleMath::Vector3 pos)
{
	m_vertical->SetPosition(pos);
}
