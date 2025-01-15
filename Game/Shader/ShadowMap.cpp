#include "pch.h"
#include "ShadowMap.h"
#include "Framework/BinaryFile.h"
#include "Framework/Resources.h"

/// <summary>
/// �C���v�b�g���C�A�E�g
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

    // ���C�g�̈ʒu
    m_lightPosition = Vector3{ 5, 50, 5 };
    m_targetPosition = Vector3::Zero;

    // ���C�g�̉�]
    m_lightRotate = Quaternion::CreateFromYawPitchRoll(
        XMConvertToRadians(0.0f), XMConvertToRadians(90.0f), 0.0f);

    m_lightTheta = 15.f;

    RECT rect = { 0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE };

    // �����_�[�e�N�X�`���̍쐬�i�V���h�E�}�b�v�p�j
    m_shadowMapRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R32_FLOAT);
    m_shadowMapRT->SetDevice(device);
    m_shadowMapRT->SetWindow(rect);

    // �f�v�X�X�e���V���̍쐬�i�V���h�E�}�b�v�p�j
    m_shadowMapDS = std::make_unique<DepthStencil>(DXGI_FORMAT_D32_FLOAT);
    m_shadowMapDS->SetDevice(device);
    m_shadowMapDS->SetWindow(rect);

    // �萔�o�b�t�@�̍쐬
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstantBuffer));	// �m�ۂ���T�C�Y�i16�̔{���Őݒ肷��j
    // GPU (�ǂݎ���p) �� CPU (�������ݐ�p) �̗�������A�N�Z�X�ł��郊�\�[�X
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// �萔�o�b�t�@�Ƃ��Ĉ���
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU�����e��ύX�ł���悤�ɂ���
    DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, m_CBuffer.ReleaseAndGetAddressOf()));

    BinaryFile vs_depth = BinaryFile::LoadFile(L"Resources/Shaders/SM_VS_Depth.cso");
    BinaryFile ps_depth = BinaryFile::LoadFile(L"Resources/Shaders/SM_PS_Depth.cso");

    //	�C���v�b�g���C�A�E�g�̍쐬
    device->CreateInputLayout(&INPUT_LAYOUT[0],
        static_cast<UINT>(INPUT_LAYOUT.size()),
        vs_depth.GetData(), vs_depth.GetSize(),
        m_inputLayout.GetAddressOf());

    // ���_�V�F�[�_�[�̍쐬�i�V���h�E�}�b�v�p�j
    DX::ThrowIfFailed(
        device->CreateVertexShader(vs_depth.GetData(), vs_depth.GetSize(), nullptr, m_VS_Depth.ReleaseAndGetAddressOf())
    );

    // �s�N�Z���V�F�[�_�[�̍쐬�i�V���h�E�}�b�v�p�j
    DX::ThrowIfFailed(
        device->CreatePixelShader(ps_depth.GetData(), ps_depth.GetSize(), nullptr, m_PS_Depth.ReleaseAndGetAddressOf())
    );

    // �T���v���[�̍쐬�i�V���h�E�}�b�v�p�j
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
    // ���\�[�X�̊��蓖�Ă���������ishadowMapRT�j
    ID3D11ShaderResourceView* nullsrv[] = { nullptr };
    context->PSSetShaderResources(1, 1, nullsrv);

    auto rtv = m_shadowMapRT->GetRenderTargetView();
    //auto srv = m_shadowMapRT->GetShaderResourceView();
    auto dsv = m_shadowMapDS->GetDepthStencilView();

    // �����_�[�^�[�Q�b�g��ύX�ishadowMapRT�j
    context->ClearRenderTargetView(rtv, SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));
    context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
    // �s�N�Z���V�F�[�_�[���烊�\�[�X���A���o�C���h
    ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
    context->PSSetShaderResources(0, 1, nullSRV);
    // �s�N�Z���V�F�[�_�[���烊�\�[�X���A���o�C���h
    context->OMSetRenderTargets(1, &rtv, dsv);

    // �r���[�|�[�g��ݒ�
    D3D11_VIEWPORT vp = { 0.0f, 0.0f, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0.0f, 1.0f };
    context->RSSetViewports(1, &vp);

    // ���C�g�̕���
    SimpleMath::Vector3 lightDir = SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 1.0f), m_lightRotate);

    // �r���[�s����쐬
    auto view = SimpleMath::Matrix::CreateLookAt(
        m_lightPosition,
        m_targetPosition,
        SimpleMath::Vector3::UnitY
    );

    // �ˉe�s����쐬
    auto proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
        XMConvertToRadians(m_lightTheta), 1.0f, 0.1f, 300.0f);

    D3D11_MAPPED_SUBRESOURCE mappedResource;

    // GPU���萔�o�b�t�@�ɑ΂��ăA�N�Z�X���s��Ȃ��悤�ɂ���
    context->Map(m_CBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    ConstantBuffer cbuffer = {};
    SimpleMath::Matrix m = view * proj;
    cbuffer.lightViewProj = XMMatrixTranspose(m);
    cbuffer.lightPosition = m_lightPosition;
    cbuffer.color = { 1,1,1,1 };

    *static_cast<ConstantBuffer*>(mappedResource.pData) = cbuffer;

    // GPU���萔�o�b�t�@�ɑ΂��ẴA�N�Z�X��������
    context->Unmap(m_CBuffer.Get(), 0);

    // �萔�o�b�t�@�̐ݒ�
    ID3D11Buffer* cbuf[] = { m_CBuffer.Get() };
    context->VSSetConstantBuffers(1, 1, cbuf);
}

void ShadowMap::RenderDepth()
{

    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    //	�C���v�b�g���C�A�E�g�̓o�^
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
    // �����_�[�^�[�Q�b�g�ƃr���[�|�[�g�����ɖ߂�
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

    // �r���[�s����쐬
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

    // �ˉe�s����쐬
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
