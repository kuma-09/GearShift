#include "pch.h"
#include "UI.h"
#include "Framework/BinaryFile.h"
#include "Framework/DeviceResources.h"

///	<summary>
///	インプットレイアウト
///	</summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> UI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

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
	

    m_spriteBatch->Begin();
	m_spriteBatch->Draw(GetTexture(), pos * value, 0, color, 0, origin, Vector2{size.x * value, size.y * value});
    m_spriteBatch->End();

}