#include "pch.h"
#include "Bloom.h"
#include "Framework/BinaryFile.h"

Graphics*                                                                Bloom::m_graphics;
std::unique_ptr<DirectX::SpriteBatch>                                    Bloom::s_spriteBatch;
DirectX::VertexPositionTexture                                           Bloom::m_vertex[4];
std::unique_ptr<DX::RenderTexture>                                       Bloom::m_offscreenRT_Bloom;
std::unique_ptr<DX::RenderTexture>                                       Bloom::m_finalRenderTexture;
std::unique_ptr<DX::RenderTexture>                                       Bloom::m_blur1RT;
std::unique_ptr<DX::RenderTexture>                                       Bloom::m_blur2RT;
std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> Bloom::m_batch;
std::unique_ptr<DirectX::BasicPostProcess>                               Bloom::m_basicPostProcess;
std::unique_ptr<DirectX::DualPostProcess>                                Bloom::m_dualPostProcess;
const float Bloom::PIXELSIZE = 16.0f;

void Bloom::Initialize()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;
    m_graphics = Graphics::GetInstance();
    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
    s_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
    RECT rect = m_graphics->GetDeviceResources()->GetOutputSize();

    // �����_�[�e�N�X�`���̍쐬�i�V�[���S�́j
    m_offscreenRT_Bloom = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
    m_offscreenRT_Bloom->SetDevice(device);
    m_offscreenRT_Bloom->SetWindow(rect);

    m_finalRenderTexture = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
    m_finalRenderTexture->SetDevice(device);
    m_finalRenderTexture->SetWindow(rect);

    // �����_�[�e�N�X�`���̍쐬�i�u���[���p�j
    rect.right  /= int(PIXELSIZE);
    rect.bottom /= int(PIXELSIZE);

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
    m_vertex[1] = { Vector3(1.0f , 1.0f,0), Vector2(1, 0) };	//�E��
    m_vertex[2] = { Vector3(-1.0f ,-1.0f,0), Vector2(0, 1) };	//����
    m_vertex[3] = { Vector3(1.0f ,-1.0f,0), Vector2(1, 1) };	//�E��
}

void Bloom::Update(float elapsedTime)
{
}

void Bloom::BeginBloom()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
    auto depthStencil = m_graphics->GetDeviceResources()->GetDepthStencilView();
    auto offscreenRTV_Bloom = m_offscreenRT_Bloom->GetRenderTargetView();

    //RECT rect = m_graphics->GetDeviceResources()->GetOutputSize();

    //m_offscreenRT_Bloom->SetWindow(rect);
    //m_finalRenderTexture->SetWindow(rect);

    // -------------------------------------------------------------------------- //
    // �����_�[�^�[�Q�b�g��ύX�ioffscreenRTV_Bloom�j
    // -------------------------------------------------------------------------- //
    ID3D11ShaderResourceView* nullsrv[] = { nullptr };
    context->PSSetShaderResources(1, 1, nullsrv);
    context->ClearRenderTargetView(offscreenRTV_Bloom, Colors::Black);
    context->OMSetRenderTargets(1, &offscreenRTV_Bloom, depthStencil);
    // -------------------------------------------------------------------------- //
}

void Bloom::EndBloom(ID3D11ShaderResourceView* srv)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    auto renderTarget = m_graphics->GetDeviceResources()->GetRenderTargetView();
    auto depthStencil = m_graphics->GetDeviceResources()->GetDepthStencilView();
    auto offscreenRTV_Bloom = m_offscreenRT_Bloom->GetRenderTargetView();
    auto offscreenSRV_Bloom = m_offscreenRT_Bloom->GetShaderResourceView();
    auto finalRTV = m_finalRenderTexture->GetRenderTargetView();

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
    { 0.0f, 0.0f, rect.right / PIXELSIZE, rect.bottom / PIXELSIZE, 0.0f, 1.0f };
    context->RSSetViewports(1, &vp_blur);

    m_basicPostProcess->SetEffect(BasicPostProcess::BloomExtract);
    m_basicPostProcess->SetBloomExtractParameter(0.45f);
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
    ID3D11ShaderResourceView* nullsrv[] = { nullptr};
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
    context->ClearRenderTargetView(finalRTV, Colors::Black);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &finalRTV, depthStencil);
    auto const viewport = m_graphics->GetDeviceResources()->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    ////// -------------------------------------------------------------------------- //
    m_dualPostProcess->SetEffect(DualPostProcess::BloomCombine);
    m_dualPostProcess->SetBloomCombineParameters(5.0f, 1.0f, 1.0f, 1.0f);
    m_dualPostProcess->SetSourceTexture(srv);
    m_dualPostProcess->SetSourceTexture2(blur1SRV);
    m_dualPostProcess->Process(context);

}

void Bloom::BloomTextureShow()
{
    using namespace DirectX::SimpleMath;

    ID3D11ShaderResourceView* srv = m_offscreenRT_Bloom->GetShaderResourceView();
    RECT rect = { 0,0,1280,720 };

    s_spriteBatch->Begin();
    s_spriteBatch->Draw(srv, Vector2{ 1280,720 }, &rect, DirectX::Colors::White, 0.0f, Vector2{ 1280,720 }, 0.25f);
    s_spriteBatch->End();
}
