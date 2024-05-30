#include "pch.h"
#include "Particle.h"
#include "cmath"

void Particle::Initialize()
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	m_graphics = Graphics::GetInstance();
	m_deviceResources = m_graphics->GetDeviceResources();
	m_inputManager = InputManager::GetInstance();

    m_resources = Resources::GetInstance();

    // �l�p�`�̒��_���W���`����c������̃R�̎��A���_���̎w���DrawQuad���g����
    m_vertices[0] = { Vector3(-1.0f , 1.0f,0),Vector4(1.f,1.f,1.f,1.f), Vector2(0.0f, 0.0f) };	//����
    m_vertices[1] = { Vector3( 1.0f , 1.0f,0),Vector4(1.f,1.f,1.f,1.f), Vector2(1.0f, 0.0f) };	//�E��
    m_vertices[2] = { Vector3(-1.0f ,-1.0f,0),Vector4(1.f,1.f,1.f,1.f), Vector2(0.0f, 1.0f) };	//����
    m_vertices[3] = { Vector3( 1.0f ,-1.0f,0),Vector4 (1.f,1.f,1.f,1.f), Vector2(1.0f, 1.0f) };	//�E��



    DirectX::CreateWICTextureFromFile(
        m_deviceResources->GetD3DDevice(),		// �f�o�C�X�R���e�L�X�g
        L"Resources/Textures/particle.png",	    // �摜�t�@�C���̃p�X
        nullptr,								// �����I�ȃe�N�X�`��
        m_texture.ReleaseAndGetAddressOf()		// �V�F�[�_���\�[�X�r���[(�\���p)
    );
}

void Particle::Update(float elapseTime)
{
    using namespace DirectX::SimpleMath;

    for (auto& position: m_positionLife)
    {
        position.lifeTime -= elapseTime;
        if (position.lifeTime <= 0)
        {
            m_positionLife.erase(m_positionLife.begin());
        }
    }

}

void Particle::Render(DirectX::SimpleMath::Matrix world)
{
    using namespace DirectX::SimpleMath;

    auto view = m_graphics->GetViewMatrix();
    auto projection = m_graphics->GetProjectionMatrix();

    auto context = m_deviceResources->GetD3DDeviceContext();

    m_positionLife.push_back({world,1.0f});

    m_graphics->GetBasicEffect()->SetTexture(m_texture.Get());




    for (auto& position: m_positionLife)
    {
        Matrix billboard = view.Invert();

        billboard._11 = 1;
        billboard._12 = 0;
        billboard._13 = 0;

        billboard._21 = 0;
        billboard._22 = 1;
        billboard._23 = 0;

        //billboard._31 = 0;
        //billboard._32 = 0;
        //billboard._33 = 1;

        billboard._41 = 0;
        billboard._42 = 0;
        billboard._43 = 0;

        billboard *= position.world;

        m_graphics->DrawPrimitiveBegin(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix(), billboard, {1,1,1,position.lifeTime});
        m_graphics->GetPrimitiveBatch()->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[3], m_vertices[2]);
        m_graphics->DrawPrimitiveEnd();
    }

}

void Particle::Render(DirectX::SimpleMath::Vector3 pos)
{
    using namespace DirectX::SimpleMath;

    auto view = m_graphics->GetViewMatrix();
    auto projection = m_graphics->GetProjectionMatrix();

    auto context = m_deviceResources->GetD3DDeviceContext();

    Matrix world = Matrix::CreateTranslation(pos);

    m_positionLife.push_back({ world,1.0f });

    m_graphics->GetBasicEffect()->SetTexture(m_texture.Get());

    Matrix billboard = view.Invert();

    billboard._11 = 1;
    billboard._12 = 0;
    billboard._13 = 0;

    //billboard._21 = 0;
    //billboard._22 = 1;
    //billboard._23 = 0;

    billboard._31 = 0;
    billboard._32 = 0;
    billboard._33 = 1;

    billboard._41 = 0;
    billboard._42 = 0;
    billboard._43 = 0;

    for (auto& position : m_positionLife)
    {
        m_graphics->DrawPrimitiveBegin(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix(), position.world * billboard, { 1,1,1,position.lifeTime });
        m_graphics->GetPrimitiveBatch()->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[3], m_vertices[2]);
        m_graphics->DrawPrimitiveEnd();
    }

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
