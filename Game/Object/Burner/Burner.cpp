#include "pch.h"	
#include "Burner.h"
#include "Game/Components/Trail.h"

// �R���X�g���N�^
Burner::Burner()
{
	m_vertical = std::make_unique<BurnerVertical>();
	m_horizon = std::make_unique<BurnerHorizon>();
}

// �f�X�g���N�^
Burner::~Burner()
{
}

// ����������
void Burner::Initialize()
{
	m_vertical->Initialize();
	m_horizon->Initialize();
}

// �X�V����
void Burner::Update(float elapsedTime, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion rotate)
{
	m_vertical->SetPosition(pos);
	m_horizon->SetPosition(pos);

	m_vertical->SetQuaternion(rotate);
	m_horizon->SetQuaternion(rotate);

	m_vertical->Update(elapsedTime);
	m_horizon->Update(elapsedTime);
}

// �g���C�����N���A
void Burner::ClearTrail()
{
	m_vertical->GetComponent<Trail>()->ClearBuffer();
	m_horizon->GetComponent<Trail>()->ClearBuffer();
}
