#include <pch.h>
#include "PostProcess.h"
#include "Framework/BinaryFile.h"

/// <summary>
/// �C���v�b�g���C�A�E�g
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
    using namespace DirectX::SimpleMath;

    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    RECT rect = m_graphics->GetDeviceResources()->GetOutputSize();

    // �����_�[�e�N�X�`���̍쐬�i�V�[���S�́j
    m_offscreenRT_Bloom = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
    m_offscreenRT_Bloom->SetDevice(device);

    m_offscreenRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
    m_offscreenRT->SetDevice(device);

    m_offscreenRT_Normal = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
    m_offscreenRT_Normal->SetDevice(device);

    m_finalRenderTexture = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
    m_finalRenderTexture->SetDevice(device);

    // �����_�[�e�N�X�`���̍쐬�i�u���[���p�j
    rect.right /= SCREENSIZE;
    rect.bottom /= SCREENSIZE;

    m_blur1RT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
    m_blur1RT->SetDevice(device);
    m_blur1RT->SetWindow(rect);

    m_blur2RT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
    m_blur2RT->SetDevice(device);
    m_blur2RT->SetWindow(rect);

    // �x�[�V�b�N�|�X�g�v���Z�X�̍쐬
    m_basicPostProcess = std::make_unique<BasicPostProcess>(device);

    // �f���A���|�X�g�v���Z�X�̍쐬
    m_dualPostProcess = std::make_unique<DualPostProcess>(device);

    // �v���~�e�B�u�o�b�`�̍쐬
    m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(context);

    m_vertex[0] = { Vector3(-1.0f , 1.0f,0), Vector2(0, 0) };	//����
    m_vertex[1] = { Vector3( 1.0f , 1.0f,0), Vector2(1, 0) };	//�E��
    m_vertex[2] = { Vector3(-1.0f ,-1.0f,0), Vector2(0, 1) };	//����
    m_vertex[3] = { Vector3( 1.0f ,-1.0f,0), Vector2(1, 1) };	//�E��


    BinaryFile vs = BinaryFile::LoadFile(L"Resources/Shaders/VS.cso");
    BinaryFile ps = BinaryFile::LoadFile(L"Resources/Shaders/PS.cso");
    BinaryFile startvs = BinaryFile::LoadFile(L"Resources/Shaders/StartNoiseVS.cso");
    BinaryFile startps = BinaryFile::LoadFile(L"Resources/Shaders/StartNoisePS.cso");
    BinaryFile noisevs = BinaryFile::LoadFile(L"Resources/Shaders/NoiseVS.cso");
    BinaryFile noiseps = BinaryFile::LoadFile(L"Resources/Shaders/NoisePS.cso");

    //	�C���v�b�g���C�A�E�g�̍쐬
    device->CreateInputLayout(&INPUT_LAYOUT[0],
        static_cast<UINT>(INPUT_LAYOUT.size()),
        vs.GetData(), vs.GetSize(),
        m_inputLayout.GetAddressOf()
    );

    device->CreateVertexShader(vs.GetData(), vs.GetSize(), nullptr, m_VS.ReleaseAndGetAddressOf());
    device->CreatePixelShader(ps.GetData(), ps.GetSize(), nullptr, m_PS.ReleaseAndGetAddressOf());
    device->CreateVertexShader(startvs.GetData(), startvs.GetSize(), nullptr, m_startVS.ReleaseAndGetAddressOf());
    device->CreatePixelShader(startps.GetData(), startps.GetSize(), nullptr, m_startPS.ReleaseAndGetAddressOf());
    device->CreateVertexShader(noisevs.GetData(), noisevs.GetSize(), nullptr, m_noiseVS.ReleaseAndGetAddressOf());
    device->CreatePixelShader(noiseps.GetData(), noiseps.GetSize(), nullptr, m_noisePS.ReleaseAndGetAddressOf());

    // �萔�o�b�t�@�̍쐬
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstantBuffer));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, m_constantBuffer.ReleaseAndGetAddressOf()));

    m_isStartNoise = true;  
    m_isNoise = false;
    m_nowTime = m_maxStartNoiseTime;
}

void PostProcess::Update(float elapsedTime)
{
    RECT rect = m_graphics->GetDeviceResources()->GetOutputSize();
    m_offscreenRT_Bloom->SetWindow(rect);
    m_offscreenRT->SetWindow(rect);
    m_offscreenRT_Normal->SetWindow(rect);
    m_finalRenderTexture->SetWindow(rect);
    if (m_isStartNoise)
    {
        m_nowTime -= elapsedTime;
        if (m_nowTime <= 0)
        {
            m_isStartNoise = false;
            m_nowTime = 0;
        }
    }
    else if (m_isNoise)
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

    auto depthStencil = m_graphics->GetDeviceResources()->GetDepthStencilView();
    auto offscreenRTV_Normal = m_offscreenRT_Normal->GetRenderTargetView();

    // -------------------------------------------------------------------------- //
    // �����_�[�^�[�Q�b�g��ύX�ioffscreenRTV_Normal�j
    // -------------------------------------------------------------------------- //
    ID3D11ShaderResourceView* nullsrv[] = { nullptr };
    context->PSSetShaderResources(1, 1, nullsrv);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->ClearRenderTargetView(m_offscreenRT_Normal->GetRenderTargetView(), Colors::Black);
    context->OMSetRenderTargets(1, &offscreenRTV_Normal, depthStencil);
    // -------------------------------------------------------------------------- //
}

void PostProcess::BeginBloom()
{
    using namespace DirectX;

    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    auto depthStencil = m_graphics->GetDeviceResources()->GetDepthStencilView();
    auto offscreenRTV_Bloom = m_offscreenRT_Bloom->GetRenderTargetView();

    // -------------------------------------------------------------------------- //
    // �����_�[�^�[�Q�b�g��ύX�ioffscreenRTV_Bloom�j
    // -------------------------------------------------------------------------- //
    ID3D11ShaderResourceView* nullsrv[] = { nullptr };
    context->PSSetShaderResources(1, 1, nullsrv);
    context->ClearRenderTargetView(m_offscreenRT_Bloom->GetRenderTargetView(), Colors::Black);
    context->OMSetRenderTargets(1, &offscreenRTV_Bloom, depthStencil);
    // -------------------------------------------------------------------------- //
}

void PostProcess::combinationRT()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    auto renderTarget = m_graphics->GetDeviceResources()->GetRenderTargetView();
    auto depthStencil = m_graphics->GetDeviceResources()->GetDepthStencilView();
    auto offscreenRTV = m_offscreenRT->GetRenderTargetView();
    auto offscreenSRV = m_offscreenRT->GetShaderResourceView();
    auto offscreenSRV_Normal = m_offscreenRT_Normal->GetShaderResourceView();
    auto offscreenSRV_Bloom = m_offscreenRT_Bloom->GetShaderResourceView();
    auto finalRTV = m_finalRenderTexture->GetRenderTargetView();
    auto finalSRV = m_finalRenderTexture->GetShaderResourceView();

    // ��ʂ̃T�C�Y���擾
    RECT rect = m_graphics->GetDeviceResources()->GetOutputSize();

    // -------------------------------------------------------------------------- //
    // Pass1 offscreen �� blur1 ���邢�����𒊏o����
    // -------------------------------------------------------------------------- //
    auto blur1RTV = m_blur1RT->GetRenderTargetView();
    auto blur1SRV = m_blur1RT->GetShaderResourceView();

    // �����_�[�^�[�Q�b�g��blur1�ɕύX����

    context->OMSetRenderTargets(1, &blur1RTV, nullptr);


    // �r���[�|�[�g��ύX����
    D3D11_VIEWPORT vp_blur =
    { 0.0f, 0.0f, rect.right / SCREENSIZE, rect.bottom / SCREENSIZE, 0.0f, 1.0f };
    context->RSSetViewports(1, &vp_blur);

    m_basicPostProcess->SetEffect(BasicPostProcess::BloomExtract);
    m_basicPostProcess->SetBloomExtractParameter(0.5f);
    m_basicPostProcess->SetSourceTexture(offscreenSRV_Bloom);
    m_basicPostProcess->Process(context);


    // -------------------------------------------------------------------------- //
    // Pass2 blur1 �� blur2 ���ɂڂ���
    // -------------------------------------------------------------------------- //
    auto blur2RTV = m_blur2RT->GetRenderTargetView();
    auto blur2SRV = m_blur2RT->GetShaderResourceView();

    // �����_�[�^�[�Q�b�g��blur2�ɕύX����
    context->OMSetRenderTargets(1, &blur2RTV, nullptr);

    m_basicPostProcess->SetEffect(BasicPostProcess::BloomBlur);
    m_basicPostProcess->SetBloomBlurParameters(true, 2.0f, 1.0f);
    m_basicPostProcess->SetSourceTexture(blur1SRV);
    m_basicPostProcess->Process(context);
    ID3D11ShaderResourceView* nullsrv[] = { nullptr };
    context->PSSetShaderResources(0, 1, nullsrv);
    // -------------------------------------------------------------------------- //
    // Pass3 blur2 �� blur1 �c�ɂڂ���
    // -------------------------------------------------------------------------- //

    // �����_�[�^�[�Q�b�g��blur1�ɕύX����
    context->OMSetRenderTargets(1, &blur1RTV, nullptr);
    m_basicPostProcess->SetEffect(BasicPostProcess::BloomBlur);
    m_basicPostProcess->SetBloomBlurParameters(false, 2.0f, 1.0f);
    m_basicPostProcess->SetSourceTexture(blur2SRV);
    m_basicPostProcess->Process(context);


    // -------------------------------------------------------------------------- //
    // Pass4 offscreen + blur1 �� �o�b�N�o�b�t�@
    // -------------------------------------------------------------------------- //
    // -------------------------------------------------------------------------- //
    // �����_�[�^�[�Q�b�g�ƃr���[�|�[�g�����ɖ߂�
    // -------------------------------------------------------------------------- //
    context->ClearRenderTargetView(offscreenRTV, Colors::Black);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &offscreenRTV, depthStencil);
    auto const viewport = m_graphics->GetDeviceResources()->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    ////// -------------------------------------------------------------------------- //
    m_dualPostProcess->SetEffect(DualPostProcess::BloomCombine);
    m_dualPostProcess->SetBloomCombineParameters(5.0f, 1.0f, 1.0f, 1.0f);
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

    auto state = m_graphics->GetCommonStates();

    // �萔�o�b�t�@���X�V
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    // GPU���萔�o�b�t�@�ɑ΂��ăA�N�Z�X���s��Ȃ��悤�ɂ���
    context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    // �萔�o�b�t�@���X�V
    ConstantBuffer cb = {};
    cb.time = m_nowTime / m_maxStartNoiseTime;
    *static_cast<ConstantBuffer*>(mappedResource.pData) = cb;
    // GPU���萔�o�b�t�@�ɑ΂��ẴA�N�Z�X��������
    context->Unmap(m_constantBuffer.Get(), 0);
    // �s�N�Z���V�F�[�_�g�p����萔�o�b�t�@��ݒ�
    ID3D11Buffer* cbuf_ps[] = { m_constantBuffer.Get() };
    context->PSSetConstantBuffers(1, 1, cbuf_ps);
    ID3D11SamplerState* sampler[1] = { state->LinearWrap() };
    context->PSSetSamplers(0, 1, sampler);
    context->RSSetState(state->CullNone());
    context->IASetInputLayout(m_inputLayout.Get());
    context->PSSetShaderResources(0, 1, &finalSRV);

    if (!m_isNoise)
    {
        if (m_isStartNoise)
        {
            context->VSSetShader(m_startVS.Get(), nullptr, 0);
            context->PSSetShader(m_startPS.Get(), nullptr, 0);
        }
        else
        {
            context->VSSetShader(m_VS.Get(), nullptr, 0);
            context->PSSetShader(m_PS.Get(), nullptr, 0);
        }
    }
    else
    {
        context->VSSetShader(m_noiseVS.Get(), nullptr, 0);
        context->PSSetShader(m_noisePS.Get(), nullptr, 0);
    }

    m_batch->Begin();
    m_batch->DrawQuad(m_vertex[0], m_vertex[1], m_vertex[3], m_vertex[2]);
    m_batch->End();

}
