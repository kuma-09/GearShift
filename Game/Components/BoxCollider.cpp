#include "pch.h"
#include "BoxCollider.h"
#include "Game/GameObject.h"
#include "Framework/Microsoft/DebugDraw.h"

BoxCollider::BoxCollider()
{
	m_graphics = Graphics::GetInstance();
	m_boudingBox = std::make_unique<DirectX::BoundingBox>();
	m_boudingBox->Center = DirectX::SimpleMath::Vector3::Zero;
	m_boudingBox->Extents = DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.5f);
}

BoxCollider::~BoxCollider()
{

}

void BoxCollider::Initialize()
{

}

void BoxCollider::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	m_boudingBox->Center = GetOwner()->GetPosition();
}

void BoxCollider::Render()
{


	auto view       = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();

	m_graphics->DrawPrimitivePositionColorBegin(view, projection, DirectX::SimpleMath::Matrix::Identity);
	DX::Draw(m_graphics->GetPrimitiveBatchPositionColor(), *m_boudingBox.get(), DirectX::Colors::Green);
	m_graphics->DrawPrimitivePositionColorEnd();
}

void BoxCollider::Finalize()
{

}

void BoxCollider::SetSize(DirectX::SimpleMath::Vector3 size)
{
	m_boudingBox->Extents = size;
}