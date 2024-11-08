#include "pch.h"
#include "HPUI.h"

HPUI::HPUI()
{
    using namespace DirectX;

    m_graphics = Graphics::GetInstance();

    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    m_spriteBatch = std::make_unique<SpriteBatch>(context);

    // 画像をロードする
    DX::ThrowIfFailed(
        CreateWICTextureFromFile(
            device,
            L"Resources/Textures/Box.png",
            nullptr,
            m_texture.ReleaseAndGetAddressOf()
        )
    );

    // 画像をロードする
    DX::ThrowIfFailed(
        CreateWICTextureFromFile(
            device,
            L"Resources/Textures/Box.png",
            nullptr,
            m_Ltexture.ReleaseAndGetAddressOf()
        )
    );
}

HPUI::~HPUI()
{
}

void HPUI::Initialize()
{
    for (int i = 0; i < 6; i++)
    {
        m_colorG[i] = 0;
        m_colorR[i] = 1;
    }
}

void HPUI::Update(std::vector<float> HP)
{
    for (int i = 0; i < 6; i++)
    {
        m_colorG[i] = HP[i];
        m_colorR[i] = 1.0f - HP[i];
    }
}

void HPUI::Render()
{
    using namespace DirectX::SimpleMath;

    RECT rect = { 0,0,960,720 };
    Vector2 pos = Vector2{ 50,50 };


    m_spriteBatch->Begin();
    m_spriteBatch->Draw(m_texture.Get() , pos + Vector2{   0, -50}  , &rect, Color(m_colorR[0], m_colorG[0], 0), 0.f, Vector2::Zero, 0.1f );
    m_spriteBatch->Draw(m_texture.Get() , pos                       , &rect, Color(m_colorR[1], m_colorG[1], 0), 0.f, Vector2::Zero, 0.1f );
    m_spriteBatch->Draw(m_Ltexture.Get(), pos + Vector2{ -50,   0}  , &rect, Color(m_colorR[2], m_colorG[2], 0), 0.f, Vector2::Zero, 0.1f );
    m_spriteBatch->Draw(m_Ltexture.Get(), pos + Vector2{  50,   0}  , &rect, Color(m_colorR[3], m_colorG[3], 0), 0.f, Vector2::Zero, 0.1f );
    m_spriteBatch->Draw(m_Ltexture.Get(), pos + Vector2{ -25,  50}  , &rect, Color(m_colorR[4], m_colorG[4], 0), 0.f, Vector2::Zero, 0.1f );
    m_spriteBatch->Draw(m_Ltexture.Get(), pos + Vector2{  25,  50}  , &rect, Color(m_colorR[5], m_colorG[5], 0), 0.f, Vector2::Zero, 0.1f );
    m_spriteBatch->End();
}