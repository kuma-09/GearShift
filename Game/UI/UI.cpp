#include "pch.h"
#include "UI.h"

UI::UI()
{
	m_graphics = Graphics::GetInstance();
}

void UI::SetTexture(wchar_t path)
{
    using namespace DirectX;

    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();

    // �摜�����[�h����
    DX::ThrowIfFailed(
        CreateWICTextureFromFile(
            device,
            L"Resources/Textures/" + path,
            nullptr,
            m_texture.ReleaseAndGetAddressOf()
        )
    );

}