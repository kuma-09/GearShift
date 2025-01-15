#include "pch.h"
#include "ShadowMap.h"
#include "Framework/BinaryFile.h"
#include "Framework/Resources.h"

/// <summary>
/// インプットレイアウト
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> ShadowMap::INPUT_LAYOUT =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

Graphics* ShadowMap::m_graphics;
std::unique_ptr<DirectX::SpriteBatch> ShadowMap::s_spriteBatch;
Microsoft::WRL::ComPtr<ID3D11InputLayout>  ShadowMap::m_inputLayout;
Microsoft::WRL::ComPtr<ID3D11SamplerState> ShadowMap::m_shadowMapSampler;
DirectX::SimpleMath::Vector3    ShadowMap::m_lightPosition;
DirectX::SimpleMath::Vector3    ShadowMap::m_targetPosition;
DirectX::SimpleMath::Quaternion ShadowMap::m_lightRotate;
float ShadowMap::m_lightTheta;
Microsoft::WRL::ComPtr<ID3D11VertexShader> ShadowMap::m_VS_Depth;
Microsoft::WRL::ComPtr<ID3D11PixelShader>  ShadowMap::m_PS_Depth;

Microsoft::WRL::ComPtr<ID3D11Buffer> ShadowMap::m_CBuffer;

std::unique_ptr<DX::RenderTexture> ShadowMap::m_shadowMapRT;
std::unique_ptr<DepthStencil> ShadowMap::m_shadowMapDS;

void ShadowMap::Initialize()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    s_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

    // ライトの位置
    m_lightPosition = Vector3{ 5, 50, 5 };
    m_targetPosition = Vector3::Zero;

    // ライトの回転
    m_lightRotate = Quaternion::CreateFromYawPitchRoll(
        XMConvertToRadians(0.0f), XMConvertToRadians(90.0f), 0.0f);

    m_lightTheta = 15.f;

    RECT rect = { 0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE };

    // レンダーテクスチャの作成（シャドウマップ用）
    m_shadowMapRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R32_FLOAT);
    m_shadowMapRT->SetDevice(device);
    m_shadowMapRT->SetWindow(rect);

    // デプスステンシルの作成（シャドウマップ用）
    m_shadowMapDS = std::make_unique<DepthStencil>(DXGI_FORMAT_D32_FLOAT);
    m_shadowMapDS->SetDevice(device);
    m_shadowMapDS->SetWindow(rect);

    // 定数バッファの作成
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstantBuffer));	// 確保するサイズ（16の倍数で設定する）
    // GPU (読み取り専用) と CPU (書き込み専用) の両方からアクセスできるリソース
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// 定数バッファとして扱う
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPUが内容を変更できるようにする
    DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, m_CBuffer.ReleaseAndGetAddressOf()));

    BinaryFile vs_depth = BinaryFile::LoadFile(L"Resources/Shaders/SM_VS_Depth.cso");
    BinaryFile ps_depth = BinaryFile::LoadFile(L"Resources/Shaders/SM_PS_Depth.cso");

    //	インプットレイアウトの作成
    device->CreateInputLayout(&INPUT_LAYOUT[0],
        static_cast<UINT>(INPUT_LAYOUT.size()),
        vs_depth.GetData(), vs_depth.GetSize(),
        m_inputLayout.GetAddressOf());

    // 頂点シェーダーの作成（シャドウマップ用）
    DX::ThrowIfFailed(
        device->CreateVertexShader(vs_depth.GetData(), vs_depth.GetSize(), nullptr, m_VS_Depth.ReleaseAndGetAddressOf())
    );

    // ピクセルシェーダーの作成（シャドウマップ用）
    DX::ThrowIfFailed(
        device->CreatePixelShader(ps_depth.GetData(), ps_depth.GetSize(), nullptr, m_PS_Depth.ReleaseAndGetAddressOf())
    );

    // サンプラーの作成（シャドウマップ用）
    D3D11_SAMPLER_DESC sampler_desc = CD3D11_SAMPLER_DESC(D3D11_DEFAULT);
    sampler_desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    sampler_desc.ComparisonFunc = D3D11_COMPARISON_LESS;
    device->CreateSamplerState(&sampler_desc, m_shadowMapSampler.ReleaseAndGetAddressOf());
}

void ShadowMap::BeginDepth()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;



    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
    // リソースの割り当てを解除する（shadowMapRT）
    ID3D11ShaderResourceView* nullsrv[] = { nullptr };
    context->PSSetShaderResources(1, 1, nullsrv);

    auto rtv = m_shadowMapRT->GetRenderTargetView();
    //auto srv = m_shadowMapRT->GetShaderResourceView();
    auto dsv = m_shadowMapDS->GetDepthStencilView();

    // レンダーターゲットを変更（shadowMapRT）
    context->ClearRenderTargetView(rtv, SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));
    context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
    // ピクセルシェーダーからリソースをアンバインド
    ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
    context->PSSetShaderResources(0, 1, nullSRV);
    // ピクセルシェーダーからリソースをアンバインド
    context->OMSetRenderTargets(1, &rtv, dsv);

    // ビューポートを設定
    D3D11_VIEWPORT vp = { 0.0f, 0.0f, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0.0f, 1.0f };
    context->RSSetViewports(1, &vp);

    // ライトの方向
    SimpleMath::Vector3 lightDir = SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 1.0f), m_lightRotate);

    // ビュー行列を作成
    auto view = SimpleMath::Matrix::CreateLookAt(
        m_lightPosition,
        m_targetPosition,
        SimpleMath::Vector3::UnitY
    );

    // 射影行列を作成
    auto proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
        XMConvertToRadians(m_lightTheta), 1.0f, 0.1f, 300.0f);

    D3D11_MAPPED_SUBRESOURCE mappedResource;

    // GPUが定数バッファに対してアクセスを行わないようにする
    context->Map(m_CBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    ConstantBuffer cbuffer = {};
    SimpleMath::Matrix m = view * proj;
    cbuffer.lightViewProj = XMMatrixTranspose(m);
    cbuffer.lightPosition = m_lightPosition;
    cbuffer.color = { 1,1,1,1 };

    *static_cast<ConstantBuffer*>(mappedResource.pData) = cbuffer;

    // GPUが定数バッファに対してのアクセスを許可する
    context->Unmap(m_CBuffer.Get(), 0);

    // 定数バッファの設定
    ID3D11Buffer* cbuf[] = { m_CBuffer.Get() };
    context->VSSetConstantBuffers(1, 1, cbuf);
}

void ShadowMap::RenderDepth()
{

    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    //	インプットレイアウトの登録
    context->IASetInputLayout(m_inputLayout.Get());
    context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
    context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);

}

void ShadowMap::EndDepth()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    // -------------------------------------------------------------------------- //
    // レンダーターゲットとビューポートを元に戻す
    // -------------------------------------------------------------------------- //
    auto renderTarget = m_graphics->GetDeviceResources()->GetRenderTargetView();
    auto depthStencil = m_graphics->GetDeviceResources()->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::Black);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    ID3D11ShaderResourceView* nullSRV[2] = { nullptr };
    context->PSSetShaderResources(0, 2, nullSRV);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);
    auto const viewport = m_graphics->GetDeviceResources()->GetScreenViewport();
    context->RSSetViewports(1, &viewport);
}

void ShadowMap::SetLightPosition(DirectX::SimpleMath::Vector3 targetPos)
{
    using namespace DirectX::SimpleMath;
    //m_lightPosition = targetPos + Vector3{ 5, 50, 5 };
    m_targetPosition = targetPos;
}

DirectX::SimpleMath::Matrix ShadowMap::GetLightView()
{
    using namespace DirectX;

    // ビュー行列を作成
    auto view = SimpleMath::Matrix::CreateLookAt(
        m_lightPosition,
        m_targetPosition,
        SimpleMath::Vector3::UnitY
    );

    return view;
}

DirectX::SimpleMath::Matrix ShadowMap::GetLightProj()
{
    using namespace DirectX;

    // 射影行列を作成
    auto proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
        DirectX::XMConvertToRadians(m_lightTheta), 1.0f, 0.1f, 300.0f);

    return proj;
}


DX::RenderTexture* ShadowMap::GetShadowRenderTexture()
{
    return m_shadowMapRT.get();
}

void ShadowMap::ShadowMapShow()
{
    using namespace DirectX::SimpleMath;

    ID3D11ShaderResourceView* srv = m_shadowMapRT->GetShaderResourceView();
    RECT rect = { 0,0,SHADOWMAP_SIZE,SHADOWMAP_SIZE };

    s_spriteBatch->Begin();
    s_spriteBatch->Draw(srv, Vector2{ 1280,0 }, &rect, DirectX::Colors::White, 0.0f, Vector2{ SHADOWMAP_SIZE,0 }, 0.5f);
    s_spriteBatch->End();
}
