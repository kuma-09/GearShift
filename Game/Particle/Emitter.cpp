#include "pch.h"
#include "Emitter.h"
#include "Framework/Easing.h"
#include <cmath>
#include "Game/GameObject.h"
#include "Game/Manager/RenderManager.h"


Emitter::Emitter()
{
    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_resources = Resources::GetInstance();
    RenderManager::Add(this);
}

Emitter::~Emitter()
{
    Finalize();
}

void Emitter::Initialize(const wchar_t* path, float size,float interval, float lifeTime)
{
    using namespace DirectX::SimpleMath;
    using namespace DirectX;

    size /= 2;

    // 四角形の頂点座標を定義する…左下基準のコの字、頂点順の指定でDrawQuadが使える
    m_vertices[0] = { Vector3(-size , size,0),Vector4(1.f,1.f,1.f,1.f), Vector2(0.0f, 0.0f) };	//左上
    m_vertices[1] = { Vector3(size , size,0) ,Vector4(1.f,1.f,1.f,1.f), Vector2(1.0f, 0.0f) };	//右上
    m_vertices[2] = { Vector3(-size ,-size,0),Vector4(1.f,1.f,1.f,1.f), Vector2(0.0f, 1.0f) };	//左下
    m_vertices[3] = { Vector3(size ,-size,0) ,Vector4(1.f,1.f,1.f,1.f), Vector2(1.0f, 1.0f) };	//右下

    DirectX::CreateWICTextureFromFile(
        m_deviceResources->GetD3DDevice(),		// デバイスコンテキスト
        path,	    // 画像ファイルのパス
        nullptr,								// 内部的なテクスチャ
        m_texture.ReleaseAndGetAddressOf()		// シェーダリソースビュー(表示用)
    );

    for (int i = 0; i < (lifeTime / interval) * 2; i++)
    {
        m_particles.push_back(std::make_unique<Particle>());
    }

    m_totalTime = 0;
    m_interval = interval;
    m_lifeTime = lifeTime;
}

void Emitter::Update(float elapseTime)
{
    using namespace DirectX::SimpleMath;

    m_totalTime += elapseTime;

    for (auto& particle : m_particles)
    {
        if (particle->GetLifeTime() > 0)
        {
            particle->Update(elapseTime);
        }
    }
}

void Emitter::Render()
{
    using namespace DirectX::SimpleMath;

    auto view = m_graphics->GetViewMatrix();
    auto projection = m_graphics->GetProjectionMatrix();

    m_graphics->GetBasicEffect()->SetTexture(m_texture.Get());
    for (auto& particle : m_particles)
    {
        if (particle->GetLifeTime() > 0)
        {
            particle->Render(&m_vertices[0], &m_vertices[1], &m_vertices[2], &m_vertices[3]);
        }
    }
}

void Emitter::Finalize()
{
    RenderManager::Remove(this);
}

void Emitter::SetParticle(DirectX::SimpleMath::Vector3 pos)
{
    if (m_totalTime >= m_interval && GetOwner()->GetVelocity() != DirectX::SimpleMath::Vector3::Zero)
    {
        for (int i = 0; i < m_particles.size(); i++)
        {
            if (m_particles[i]->GetLifeTime() <= 0)
            {
                m_totalTime = 0;
                m_particles[i]->Initialize(pos, m_lifeTime, DirectX::XMConvertToRadians(rand() % 360));
                return;
            }
        }
    }

    //if (m_totalTime >= m_interval && GetOwner()->GetVelocity() != DirectX::SimpleMath::Vector3::Zero)
    //{
    //    m_totalTime = 0;
    //    m_particles.emplace_back(std::make_unique<Particle>());
    //    m_particles.back()->Initialize(pos, m_lifeTime,DirectX::XMConvertToRadians(rand() % 360));
    //}
}
