#include "pch.h"
#include "Emitter.h"
#include <cmath>

void Emitter::Initialize()
{
    using namespace DirectX::SimpleMath;
    using namespace DirectX;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();

    m_resources = Resources::GetInstance();

    // �l�p�`�̒��_���W���`����c������̃R�̎��A���_���̎w���DrawQuad���g����
    m_vertices[0] = { Vector3(-0.5f , 0.5f,0),Vector4(1.f,1.f,1.f,1.f), Vector2(0.0f, 0.0f) };	//����
    m_vertices[1] = { Vector3(0.5f , 0.5f,0),Vector4(1.f,1.f,1.f,1.f), Vector2(1.0f, 0.0f) };	//�E��
    m_vertices[2] = { Vector3(-0.5f ,-0.5f,0),Vector4(1.f,1.f,1.f,1.f), Vector2(0.0f, 1.0f) };	//����
    m_vertices[3] = { Vector3(0.5f ,-0.5f,0),Vector4(1.f,1.f,1.f,1.f), Vector2(1.0f, 1.0f) };	//�E��



    DirectX::CreateWICTextureFromFile(
        m_deviceResources->GetD3DDevice(),		// �f�o�C�X�R���e�L�X�g
        L"Resources/Textures/smoke_white.png",	    // �摜�t�@�C���̃p�X
        nullptr,								// �����I�ȃe�N�X�`��
        m_texture.ReleaseAndGetAddressOf()		// �V�F�[�_���\�[�X�r���[(�\���p)
    );

    m_totalTime = 0;
}

void Emitter::Update(float elapseTime)
{
    using namespace DirectX::SimpleMath;

    m_totalTime += elapseTime;
    for (auto& particle : m_particles)
    {
        particle->Update(elapseTime);
    }


}

void Emitter::Render(DirectX::SimpleMath::Matrix world)
{
    using namespace DirectX::SimpleMath;

    auto view = m_graphics->GetViewMatrix();
    auto projection = m_graphics->GetProjectionMatrix();

    auto context = m_deviceResources->GetD3DDeviceContext();

    if (m_totalTime >= m_interval)
    {
        m_totalTime = 0;
        m_particles.push_back(std::make_unique<Particle>());
        //m_particles.back()->Initialize(world);
    }


    m_graphics->GetBasicEffect()->SetTexture(m_texture.Get());

    for (auto& particle : m_particles)
    {
        particle->Render(&m_vertices[0], &m_vertices[1], &m_vertices[2], &m_vertices[3], m_texture);
    }
}

void Emitter::Render(DirectX::SimpleMath::Vector3 pos)
{
    using namespace DirectX::SimpleMath;

    auto view = m_graphics->GetViewMatrix();
    auto projection = m_graphics->GetProjectionMatrix();

    auto context = m_deviceResources->GetD3DDeviceContext();

    if (m_totalTime >= m_interval)
    {
        m_totalTime = 0;
        m_particles.push_back(std::make_unique<Particle>());
        m_particles.back()->Initialize(pos);
    }


    m_graphics->GetBasicEffect()->SetTexture(m_texture.Get());

    for (auto& particle : m_particles)
    {
        particle->Render(&m_vertices[0], &m_vertices[1], &m_vertices[2], &m_vertices[3], m_texture);
    }
}

void Emitter::Finalize()
{
    delete m_graphics;
    m_graphics = nullptr;
    delete m_deviceResources;
    m_deviceResources = nullptr;
    delete m_inputManager;
    m_inputManager = nullptr;
}
