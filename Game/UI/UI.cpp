#include "pch.h"
#include "UI.h"
#include "Framework/BinaryFile.h"
#include "Framework/DeviceResources.h"

UI::UI(const wchar_t* path)
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
            path,
            nullptr,
            m_texture.ReleaseAndGetAddressOf()
        )
    );

}

UI::~UI()
{

}

void UI::Initialize()
{
}

void UI::Render(DirectX::SimpleMath::Vector2 pos, DirectX::XMVECTORF32 color, DirectX::SimpleMath::Vector2 origin, DirectX::SimpleMath::Vector2 size)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	RECT windowsize = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
	int x, y;
	Graphics::GetInstance()->GetScreenSize(x, y);
	float value = float(windowsize.right) / x;
	
	auto state = m_graphics->GetCommonStates();


	//	半透明描画指定
	ID3D11BlendState* blendstate = state->NonPremultiplied();


	m_spriteBatch->Begin(SpriteSortMode_BackToFront, blendstate);
	m_spriteBatch->Draw(GetTexture(), pos * value, 0, color, 0, origin, Vector2{size.x * value, size.y * value});
    m_spriteBatch->End();

}

void UI::SetTexture(const wchar_t* path)
{
    using namespace DirectX;
    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    // 画像をロードする
    DX::ThrowIfFailed(
        CreateWICTextureFromFile(
            device,
            path,
            nullptr,
            m_texture.ReleaseAndGetAddressOf()
        )
    );
}
