#include "pch.h"
#include "Shadow.h"
#include "Framework/BinaryFile.h"
#include "Framework/Resources.h"

/// <summary>
/// インプットレイアウト
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> Shadow::INPUT_LAYOUT =
{
    { "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR" ,   0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

Graphics* Shadow::m_graphics;
Microsoft::WRL::ComPtr<ID3D11InputLayout>  Shadow::m_inputLayout;
Microsoft::WRL::ComPtr<ID3D11SamplerState> Shadow::m_shadowMapSampler;
DirectX::SimpleMath::Vector3    Shadow::m_lightPosition;
DirectX::SimpleMath::Quaternion Shadow::m_lightRotate;
float Shadow::m_lightTheta;
Microsoft::WRL::ComPtr<ID3D11VertexShader> Shadow::m_VS_Depth;
Microsoft::WRL::ComPtr<ID3D11PixelShader>  Shadow::m_PS_Depth;
Microsoft::WRL::ComPtr<ID3D11VertexShader> Shadow::m_VS;
Microsoft::WRL::ComPtr<ID3D11PixelShader>  Shadow::m_PS;
Microsoft::WRL::ComPtr<ID3D11PixelShader>  Shadow::m_PS_Tex;

Microsoft::WRL::ComPtr<ID3D11Buffer> Shadow::m_CBuffer;
Microsoft::WRL::ComPtr<ID3D11Buffer> Shadow::m_CBuffer2;
std::unique_ptr<DX::RenderTexture> Shadow::m_shadowMapRT;
std::unique_ptr<DepthStencil>      Shadow::m_shadowMapDS;

ID3D11RenderTargetView*   Shadow::m_rtv;
ID3D11ShaderResourceView* Shadow::m_srv;
ID3D11DepthStencilView*   Shadow::m_dsv;

void Shadow::Initialize()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    // ライトの位置
    m_lightPosition = Vector3{ 0,100, -30 };

    // ライトの回転
    m_lightRotate = Quaternion::CreateFromYawPitchRoll(
        XMConvertToRadians(0.0f), XMConvertToRadians(80.0f), 0.0f);

    m_lightTheta = 120.0f;

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

    // 定数バッファの作成
    bufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstantBuffer2));	// 確保するサイズ（16の倍数で設定する）
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;	// GPUの読み取りと書き込みが可能な一般的なリソース
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// 定数バッファとして扱う
    bufferDesc.CPUAccessFlags = 0;	// CPUはアクセスしないので0
    DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, m_CBuffer2.ReleaseAndGetAddressOf()));

    // 定数バッファの内容更新
    ConstantBuffer2 cb = {};
    cb.fCosTheta = cosf(XMConvertToRadians(m_lightTheta / 2.0f));
    context->UpdateSubresource(m_CBuffer2.Get(), 0, nullptr, &cb, 0, 0);

    BinaryFile vs_depth = BinaryFile::LoadFile(L"Resources/Shaders/SM_VS_Depth.cso");
    BinaryFile ps_depth = BinaryFile::LoadFile(L"Resources/Shaders/SM_PS_Depth.cso");
    BinaryFile vs = BinaryFile::LoadFile(L"Resources/Shaders/SM_VS.cso");
    BinaryFile ps = BinaryFile::LoadFile(L"Resources/Shaders/SM_PS.cso");
    BinaryFile ps_tex = BinaryFile::LoadFile(L"Resources/Shaders/SM_PS_Tex.cso");

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

    // 頂点シェーダーの作成（シャドウマップ用）
    DX::ThrowIfFailed(
        device->CreateVertexShader(vs.GetData(), vs.GetSize(), nullptr, m_VS.ReleaseAndGetAddressOf())
    );

    // ピクセルシェーダーの作成（シャドウマップ用）
    DX::ThrowIfFailed(
        device->CreatePixelShader(ps.GetData(), ps.GetSize(), nullptr, m_PS.ReleaseAndGetAddressOf())
    );

    // ピクセルシェーダーの作成（シャドウマップ用）
    DX::ThrowIfFailed(
        device->CreatePixelShader(ps_tex.GetData(), ps_tex.GetSize(), nullptr, m_PS_Tex.ReleaseAndGetAddressOf())
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

void Shadow::BeginDepth()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;



    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
    // リソースの割り当てを解除する（shadowMapRT）
    ID3D11ShaderResourceView* nullsrv[] = { nullptr };
    context->PSSetShaderResources(1, 1, nullsrv);

    m_rtv = m_shadowMapRT->GetRenderTargetView();
    m_srv = m_shadowMapRT->GetShaderResourceView();
    m_dsv = m_shadowMapDS->GetDepthStencilView();

    // レンダーターゲットを変更（shadowMapRT）
    context->ClearRenderTargetView(m_rtv, SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));
    context->ClearDepthStencilView(m_dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
    // ピクセルシェーダーからリソースをアンバインド
    ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
    context->PSSetShaderResources(0, 1, nullSRV);
    // ピクセルシェーダーからリソースをアンバインド
    context->OMSetRenderTargets(1, &m_rtv, m_dsv);

    // ビューポートを設定
    D3D11_VIEWPORT vp = { 0.0f, 0.0f, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0.0f, 1.0f };
    context->RSSetViewports(1, &vp);
}

void Shadow::RenderDepth()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    //	インプットレイアウトの登録
    context->IASetInputLayout(m_inputLayout.Get());
    context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
    context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);

}

void Shadow::EndDepth()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    // -------------------------------------------------------------------------- //
    // レンダーターゲットとビューポートを元に戻す
    // -------------------------------------------------------------------------- //
    auto renderTarget = m_graphics->GetDeviceResources()->GetRenderTargetView();
    auto depthStencil = m_graphics->GetDeviceResources()->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    ID3D11ShaderResourceView* nullSRV[2] = { nullptr };
    context->PSSetShaderResources(0, 2, nullSRV);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);
    auto const viewport = m_graphics->GetDeviceResources()->GetScreenViewport();
    context->RSSetViewports(1, &viewport);
}

void Shadow::Draw(bool texture, DirectX::XMVECTORF32 color)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
    auto state = m_graphics->GetCommonStates();

    // ライトの方向
    SimpleMath::Vector3 lightDir = SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 1.0f), m_lightRotate);

    // ビュー行列を作成
    auto view = SimpleMath::Matrix::CreateLookAt(
        m_lightPosition,
        m_lightPosition + lightDir,
        SimpleMath::Vector3::UnitY
    );

    // 射影行列を作成
    auto proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
        XMConvertToRadians(m_lightTheta), 1.0f, 0.1f, 1000.0f);

    D3D11_MAPPED_SUBRESOURCE mappedResource;

    // GPUが定数バッファに対してアクセスを行わないようにする
    context->Map(m_CBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    ConstantBuffer cbuffer = {};
    SimpleMath::Matrix m = view * proj;
    cbuffer.lightViewProj = XMMatrixTranspose(m);
    cbuffer.lightPosition = m_lightPosition;
    cbuffer.lightDirection = lightDir;
    cbuffer.lightAmbient = SimpleMath::Color(0.3f, 0.3f, 0.3f);
    cbuffer.color = color;

    *static_cast<ConstantBuffer*>(mappedResource.pData) = cbuffer;

    // GPUが定数バッファに対してのアクセスを許可する
    context->Unmap(m_CBuffer.Get(), 0);

    // 定数バッファの設定
    ID3D11Buffer* cbuf[] = { m_CBuffer.Get(),m_CBuffer2.Get() };
    context->VSSetConstantBuffers(1, 1, cbuf);
    context->PSSetConstantBuffers(1, 2, cbuf);

    // 作成したシャドウマップをリソースとして設定
    context->PSSetShaderResources(1, 1, &m_srv);

    // テクスチャサンプラーの設定
    ID3D11SamplerState* samplers[] = { state->LinearWrap(), m_shadowMapSampler.Get() };
    context->PSSetSamplers(0, 2, samplers);

    //	インプットレイアウトの登録
    context->IASetInputLayout(m_inputLayout.Get());

    // シェーダーの設定
    context->VSSetShader(m_VS.Get(), nullptr, 0);
    if (texture)
    {
        context->PSSetShader(m_PS_Tex.Get(), nullptr, 0);
    }
    else
    {
        context->PSSetShader(m_PS.Get(), nullptr, 0);
    }
}