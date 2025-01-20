#include "pch.h"
#include "RemainingEnemyUI.h"

RemainingEnemyUI::RemainingEnemyUI()
{
	m_nowNumber = std::make_unique<Number>();
	m_maxNumber = std::make_unique<Number>();
	m_slash     = std::make_unique<Number>();
}

RemainingEnemyUI::~RemainingEnemyUI()
{
}

void RemainingEnemyUI::Initialize(DirectX::SimpleMath::Vector2 pos, int num)
{
	m_nowNumber->Initialize(pos - DirectX::SimpleMath::Vector2{75,0});
	m_maxNumber->Initialize(pos + DirectX::SimpleMath::Vector2{75,0});
	m_slash->Initialize(pos);

	m_nowNumber->SetNum(num);
	m_maxNumber->SetNum(num);
}

void RemainingEnemyUI::Update(int num)
{
	m_nowNumber->SetNum(num);
}

void RemainingEnemyUI::Render()
{
	m_nowNumber->Render();
	m_maxNumber->Render();
	m_slash->RenderSlash();
}
