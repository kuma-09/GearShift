#include "pch.h"
#include "UI.h"

UI::UI(const wchar_t* path, DirectX::SimpleMath::Vector2 pos)
{
    using namespace DirectX;


    m_graphics = Graphics::GetInstance();
    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    m_spriteBatch = std::make_unique<SpriteBatch>(context);

    

    // ‰æ‘œ‚ðƒ[ƒh‚·‚é
    DX::ThrowIfFailed(
        CreateWICTextureFromFile(
            device,
            path,
            nullptr,
            m_texture.ReleaseAndGetAddressOf()
        )
    );

    m_position = pos;
}

UI::~UI()
{

}

void UI::Initialize()
{

}

void UI::Render()
{
    m_spriteBatch->Begin();
    m_spriteBatch->Draw(GetTexture(),m_position);
    m_spriteBatch->End();
}