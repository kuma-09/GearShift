#include "pch.h"	
#include "Burner.h"
#include "Game/Components/Trail.h"

// コンストラクタ
Burner::Burner()
{
	m_vertical = std::make_unique<BurnerVertical>();
	m_horizon = std::make_unique<BurnerHorizon>();
}

// デストラクタ
Burner::~Burner()
{
}

// 初期化処理
void Burner::Initialize()
{
	m_vertical->Initialize();
	m_horizon->Initialize();
}

// 更新処理
void Burner::Update(float elapsedTime, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion rotate)
{
	m_vertical->SetPosition(pos);
	m_horizon->SetPosition(pos);

	m_vertical->SetQuaternion(rotate);
	m_horizon->SetQuaternion(rotate);

	m_vertical->Update(elapsedTime);
	m_horizon->Update(elapsedTime);
}

// トレイルをクリア
void Burner::ClearTrail()
{
	m_vertical->GetComponent<Trail>()->ClearBuffer();
	m_horizon->GetComponent<Trail>()->ClearBuffer();
}
