#include "pch.h"
#include "Enemy.h"

void Enemy::Initialize()
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	m_graphics = Graphics::GetInstance();
	m_deviceResources = m_graphics->GetDeviceResources();

    m_resources = Resources::GetInstance();

	m_position = Vector3(0, 0, 0);

}

void Enemy::Update()
{

}

void Enemy::Render()
{
    using namespace DirectX::SimpleMath;

    auto view = m_graphics->GetViewMatrix();
    auto projection = m_graphics->GetProjectionMatrix();

    auto context = m_deviceResources->GetD3DDeviceContext();


    m_graphics->DrawPrimitiveBegin(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());

    Matrix world = Matrix::Identity;

    world = Matrix::CreateRotationY(0);
    world *= Matrix::CreateTranslation(m_position);

    m_resources->GetModel()->Draw(context, *m_graphics->GetCommonStates(), world, m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());

    m_graphics->DrawPrimitiveEnd();
}

void Enemy::Finalize()
{

}