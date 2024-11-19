#include "pch.h"
#include "Particle.h"
#include "cmath"

void Particle::Initialize(DirectX::SimpleMath::Vector3 pos, float lifeTime)
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	m_graphics = Graphics::GetInstance();
	m_deviceResources = m_graphics->GetDeviceResources();
	m_inputManager = InputManager::GetInstance();

    m_resources = Resources::GetInstance();

    m_pos = pos;

    m_lifeTime = lifeTime;
    m_maxTime = lifeTime;
}

void Particle::Update(float elapseTime)
{
    using namespace DirectX::SimpleMath;
    
    m_lifeTime -= elapseTime;
    if (m_lifeTime <= 0)
    {
        //delete this;
    }

}

void Particle::Render(
    DirectX::VertexPositionColorTexture* v1,
    DirectX::VertexPositionColorTexture* v2,
    DirectX::VertexPositionColorTexture* v3,
    DirectX::VertexPositionColorTexture* v4,
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
    using namespace DirectX::SimpleMath;

    auto view = m_graphics->GetViewMatrix();
    auto projection = m_graphics->GetProjectionMatrix();

    m_graphics->GetBasicEffect()->SetTexture(texture.Get());

    Matrix billboard = view.Invert();
    billboard._41 = 0;
    billboard._42 = 0;
    billboard._43 = 0;

    Matrix world = Matrix::CreateScale(5 - m_lifeTime * 5);
    world *= Matrix::CreateTranslation(m_pos);

    billboard *= world;


    m_graphics->DrawPrimitiveBegin(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix(),billboard, {1,1,1,m_lifeTime / m_maxTime});
    m_graphics->GetPrimitiveBatch()->DrawQuad(*v1, *v2, *v4,*v3);
    m_graphics->DrawPrimitiveEnd();
}


void Particle::Finalize()
{
    delete m_graphics;
    m_graphics = nullptr;
    delete m_deviceResources;
    m_deviceResources = nullptr;
    delete m_inputManager;
    m_inputManager = nullptr;
}
