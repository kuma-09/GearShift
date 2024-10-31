#include <pch.h>
#include "PostProcess.h"
#include "Framework/BinaryFile.h"

/// <summary>
/// インプットレイアウト
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> PostProcess::INPUT_LAYOUT =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

PostProcess::PostProcess()
{
    m_graphics = Graphics::GetInstance();
}

PostProcess::~PostProcess()
{
}

void PostProcess::Initialize()
{
    using namespace DirectX;

    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    // レンダーテクスチャの作成（シーン全体）
    m_offscreenRT_Bloom = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
    m_offscreenRT_Bloom->SetDevice(device);
    RECT rect = m_graphics->GetDeviceResources()->GetOutputSize();
    m_offscreenRT_Bloom->SetWindow(rect);

    m_offscreenRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
    m_offscreenRT->SetDevice(device);
    m_offscreenRT->SetWindow(rect);

    m_offscreenRT_Normal = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
    m_offscreenRT_Normal->SetDevice(device);
    m_offscreenRT_Normal->SetWindow(rect);

    m_finalRenderTexture = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
    m_finalRenderTexture->SetDevice(device);
    m_finalRenderTexture->SetWindow(rect);

    // レンダーテクスチャの作成（ブルーム用）
    rect.right /= 2;
    rect.bottom /= 2;

    m_blur1RT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
    m_blur1RT->SetDevice(device);
    m_blur1RT->SetWindow(rect);

    m_blur2RT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
    m_blur2RT->SetDevice(device);
    m_blur2RT->SetWindow(rect);

    // ベーシックポストプロセスの作成
    m_basicPostProcess = std::make_unique<BasicPostProcess>(device);

    // デュアルポストプロセスの作成
    m_dualPostProcess = std::make_unique<DualPostProcess>(device);

    m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

    BinaryFile vs = BinaryFile::LoadFile(L"Resources/Shaders/NoiseVS.cso");
    BinaryFile ps = BinaryFile::LoadFile(L"Resources/Shaders/NoisePS.cso");

    //	インプットレイアウトの作成
    device->CreateInputLayout(&INPUT_LAYOUT[0],
        static_cast<UINT>(INPUT_LAYOUT.size()),
        vs.GetData(), vs.GetSize(),
        m_inputLayout.GetAddressOf()
    );

    DX::ThrowIfFailed(
        device->CreateVertexShader(vs.GetData(), vs.GetSize(), nullptr, m_noiseVS.ReleaseAndGetAddressOf())
    );

    DX::ThrowIfFailed(
        device->CreatePixelShader(ps.GetData(), ps.GetSize(), nullptr, m_noisePS.ReleaseAndGetAddressOf())
    );

    m_isNoise = false;
    m_nowTime = 0;
}

void PostProcess::Update(float elapsedTime)
{
    if (m_isNoise)
    {
        m_nowTime += elapsedTime;
        if (m_nowTime >= m_maxNoiseTime)
        {
            m_isNoise = false;
            m_nowTime = 0;
        }
    }
}

void PostProcess::BeginNormal()
{
    using namespace DirectX;

    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    auto renderTarget = m_graphics->GetDeviceResources()->GetRenderTargetView();
    auto depthStencil = m_graphics->GetDeviceResources()->GetDepthStencilView();
    auto offscreenRTV = m_offscreenRT->GetRenderTargetView();
    auto offscreenSRV = m_offscreenRT->GetShaderResourceView();
    auto offscreenRTV_Normal = m_offscreenRT_Normal->GetRenderTargetView();
    auto offscreenSRV_Normal = m_offscreenRT_Normal->GetShaderResourceView();

    // -------------------------------------------------------------------------- //
    // レンダーターゲットを変更（offscreenRTV_Normal）
    // -------------------------------------------------------------------------- //
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->ClearRenderTargetView(m_offscreenRT_Normal->GetRenderTargetView(), Colors::Black);
    context->OMSetRenderTargets(1, &offscreenRTV_Normal, depthStencil);
    // -------------------------------------------------------------------------- //
}

void PostProcess::BeginBloom()
{
    using namespace DirectX;

    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    auto renderTarget = m_graphics->GetDeviceResources()->GetRenderTargetView();
    auto depthStencil = m_graphics->GetDeviceResources()->GetDepthStencilView();
    auto offscreenRTV_Bloom = m_offscreenRT_Bloom->GetRenderTargetView();
    auto offscreenSRV_Bloom = m_offscreenRT_Bloom->GetShaderResourceView();

    // -------------------------------------------------------------------------- //
    // レンダーターゲットを変更（offscreenRTV_Bloom）
    // -------------------------------------------------------------------------- //
    context->ClearRenderTargetView(m_offscreenRT_Bloom->GetRenderTargetView(), Colors::Black);
    context->OMSetRenderTargets(1, &offscreenRTV_Bloom, depthStencil);
    // -------------------------------------------------------------------------- //
}

void PostProcess::combinationRT()
{
    using namespace DirectX;

    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    auto renderTarget = m_graphics->GetDeviceResources()->GetRenderTargetView();
    auto depthStencil = m_graphics->GetDeviceResources()->GetDepthStencilView();
    auto offscreenRTV = m_offscreenRT->GetRenderTargetView();
    auto offscreenSRV = m_offscreenRT->GetShaderResourceView();
    auto offscreenRTV_Normal = m_offscreenRT_Normal->GetRenderTargetView();
    auto offscreenSRV_Normal = m_offscreenRT_Normal->GetShaderResourceView();
    auto offscreenRTV_Bloom = m_offscreenRT_Bloom->GetRenderTargetView();
    auto offscreenSRV_Bloom = m_offscreenRT_Bloom->GetShaderResourceView();
    auto finalRTV = m_finalRenderTexture->GetRenderTargetView();
    auto finalSRV = m_finalRenderTexture->GetShaderResourceView();

    // 画面のサイズを取得
    RECT rect = m_graphics->GetDeviceResources()->GetOutputSize();

    // -------------------------------------------------------------------------- //
    // Pass1 offscreen → blur1 明るい部分を抽出する
    // -------------------------------------------------------------------------- //
    auto blur1RTV = m_blur1RT->GetRenderTargetView();
    auto blur1SRV = m_blur1RT->GetShaderResourceView();

    // レンダーターゲットをblur1に変更する

    context->OMSetRenderTargets(1, &blur1RTV, nullptr);
    ID3D11ShaderResourceView* nullsrv[] = { nullptr };
    context->PSSetShaderResources(1, 1, nullsrv);

    // ビューポートを変更する
    D3D11_VIEWPORT vp_blur =
    { 0.0f, 0.0f, rect.right / 2.0f, rect.bottom / 2.0f, 0.0f, 1.0f };
    context->RSSetViewports(1, &vp_blur);

    m_basicPostProcess->SetEffect(BasicPostProcess::BloomExtract);
    m_basicPostProcess->SetBloomExtractParameter(0.25f);
    m_basicPostProcess->SetSourceTexture(offscreenSRV_Bloom);
    m_basicPostProcess->Process(context);


    // -------------------------------------------------------------------------- //
    // Pass2 blur1 → blur2 横にぼかす
    // -------------------------------------------------------------------------- //
    auto blur2RTV = m_blur2RT->GetRenderTargetView();
    auto blur2SRV = m_blur2RT->GetShaderResourceView();

    // レンダーターゲットをblur2に変更する
    context->OMSetRenderTargets(1, &blur2RTV, nullptr);

    m_basicPostProcess->SetEffect(BasicPostProcess::BloomBlur);
    m_basicPostProcess->SetBloomBlurParameters(true, 4.0f, 1.0f);
    m_basicPostProcess->SetSourceTexture(blur1SRV);
    m_basicPostProcess->Process(context);

    // -------------------------------------------------------------------------- //
    // Pass3 blur2 → blur1 縦にぼかす
    // -------------------------------------------------------------------------- //

    // レンダーターゲットをblur1に変更する
    context->OMSetRenderTargets(1, &blur1RTV, nullptr);
    m_basicPostProcess->SetEffect(BasicPostProcess::BloomBlur);
    m_basicPostProcess->SetBloomBlurParameters(false, 4.0f, 1.0f);
    m_basicPostProcess->SetSourceTexture(blur2SRV);
    m_basicPostProcess->Process(context);



    // -------------------------------------------------------------------------- //
    // Pass4 offscreen + blur1 → バックバッファ
    // -------------------------------------------------------------------------- //
    // -------------------------------------------------------------------------- //
    // レンダーターゲットとビューポートを元に戻す
    // -------------------------------------------------------------------------- //
    context->ClearRenderTargetView(offscreenRTV, Colors::Black);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    context->OMSetRenderTargets(1, &offscreenRTV, depthStencil);
    auto const viewport = m_graphics->GetDeviceResources()->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    ////// -------------------------------------------------------------------------- //
    m_dualPostProcess->SetEffect(DualPostProcess::BloomCombine);
    m_dualPostProcess->SetBloomCombineParameters(10.0f, 1.0f, 0.0f, 1.0f);
    m_dualPostProcess->SetSourceTexture(offscreenSRV_Bloom);
    m_dualPostProcess->SetSourceTexture2(blur1SRV);
    m_dualPostProcess->Process(context);


    context->ClearRenderTargetView(finalRTV, Colors::Black);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &finalRTV, depthStencil);


    m_dualPostProcess->SetEffect(DualPostProcess::Merge);
    m_dualPostProcess->SetMergeParameters(1, 1);
    m_dualPostProcess->SetSourceTexture(offscreenSRV_Normal);
    m_dualPostProcess->SetSourceTexture2(offscreenSRV);
    m_dualPostProcess->Process(context);

    context->ClearRenderTargetView(renderTarget, Colors::Black);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    //auto state = m_graphics->GetCommonStates();

    //if (m_isNoise)
    //{
    //    m_spriteBatch->Begin(SpriteSortMode_Deferred,
    //        nullptr, state->LinearWrap(), nullptr, nullptr, [&]
    //        {
    //            context->IASetInputLayout(m_inputLayout.Get());
    //            context->PSSetShaderResources(0, 1, &finalSRV);
    //            context->VSSetShader(m_noiseVS.Get(), nullptr, 0);
    //            context->PSSetShader(m_noisePS.Get(), nullptr, 0);
    //        }
    //    );
    //}
    //else
    //{

    //}
    m_spriteBatch->Begin();
    m_spriteBatch->Draw(finalSRV, DirectX::SimpleMath::Vector2::Zero);
    m_spriteBatch->End();
}
