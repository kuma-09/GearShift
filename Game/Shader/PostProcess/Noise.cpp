#include <pch.h>
#include "Noise.h"
#include "Framework/BinaryFile.h"

/// <summary>
/// �C���v�b�g���C�A�E�g
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> Noise::INPUT_LAYOUT =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
Graphics* Noise::m_graphics;
Microsoft::WRL::ComPtr<ID3D11InputLayout> Noise::m_inputLayout;
Microsoft::WRL::ComPtr<ID3D11Buffer> Noise::m_constantBuffer;
DirectX::VertexPositionTexture Noise::m_vertex[4];
std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> Noise::m_batch;
Microsoft::WRL::ComPtr<ID3D11VertexShader> Noise::m_noiseVS;
Microsoft::WRL::ComPtr<ID3D11PixelShader>  Noise::m_noisePS;
bool        Noise::m_isNoise;
float       Noise::m_nowTime;
const float Noise::PIXELSIZE = 4.0f;
const float Noise::m_maxNoiseTime = 0.25f;

void Noise::Initialize()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    RECT rect = m_graphics->GetDeviceResources()->GetOutputSize();

    // �v���~�e�B�u�o�b�`�̍쐬
    m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(context);

    m_vertex[0] = { Vector3(-1.0f , 1.0f,0), Vector2(0, 0) };	//����
    m_vertex[1] = { Vector3(1.0f , 1.0f,0), Vector2(1, 0) };	//�E��
    m_vertex[2] = { Vector3(-1.0f ,-1.0f,0), Vector2(0, 1) };	//����
    m_vertex[3] = { Vector3(1.0f ,-1.0f,0), Vector2(1, 1) };	//�E��

    BinaryFile noisevs = BinaryFile::LoadFile(L"Resources/Shaders/NoiseVS.cso");
    BinaryFile noiseps = BinaryFile::LoadFile(L"Resources/Shaders/NoisePS.cso");

    //	�C���v�b�g���C�A�E�g�̍쐬
    device->CreateInputLayout(&INPUT_LAYOUT[0],
        static_cast<UINT>(INPUT_LAYOUT.size()),
        noisevs.GetData(), noisevs.GetSize(),
        m_inputLayout.GetAddressOf()
    );

    device->CreateVertexShader(noisevs.GetData(), noisevs.GetSize(), nullptr, m_noiseVS.ReleaseAndGetAddressOf());
    device->CreatePixelShader(noiseps.GetData(), noiseps.GetSize(), nullptr, m_noisePS.ReleaseAndGetAddressOf());

    m_isNoise = false;

    // �萔�o�b�t�@�̍쐬
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstantBuffer));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, m_constantBuffer.ReleaseAndGetAddressOf()));
}

void Noise::Update(float elapsedTime)
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


void Noise::ApplyNoise(ID3D11ShaderResourceView* srv)
{
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
    auto states  = m_graphics->GetCommonStates();
    auto renderTarget = m_graphics->GetDeviceResources()->GetRenderTargetView();


    // �萔�o�b�t�@���X�V
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    // GPU���萔�o�b�t�@�ɑ΂��ăA�N�Z�X���s��Ȃ��悤�ɂ���
    context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    // �萔�o�b�t�@���X�V
    ConstantBuffer cb = {};
    cb.time = m_nowTime / m_maxNoiseTime;
    *static_cast<ConstantBuffer*>(mappedResource.pData) = cb;
    // GPU���萔�o�b�t�@�ɑ΂��ẴA�N�Z�X��������
    context->Unmap(m_constantBuffer.Get(), 0);
    // �s�N�Z���V�F�[�_�g�p����萔�o�b�t�@��ݒ�
    ID3D11Buffer* cbuf_ps[] = { m_constantBuffer.Get() };
    context->PSSetConstantBuffers(1, 1, cbuf_ps);
    ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
    context->PSSetSamplers(0, 1, sampler);
    context->RSSetState(states->CullNone());

    context->ClearRenderTargetView(renderTarget,DirectX::Colors::CornflowerBlue);
    context->OMSetRenderTargets(1, &renderTarget, nullptr);
    context->RSSetState(states->CullNone());						// �J�����O

    context->IASetInputLayout(m_inputLayout.Get());
    context->PSSetShaderResources(0, 1, &srv);
    context->VSSetShader(m_noiseVS.Get(), nullptr, 0);
    context->PSSetShader(m_noisePS.Get(), nullptr, 0);

    m_batch->Begin();
    m_batch->DrawQuad(m_vertex[0], m_vertex[1], m_vertex[3], m_vertex[2]);
    m_batch->End();

    context->VSSetShader(nullptr, nullptr, 0);
    context->PSSetShader(nullptr, nullptr, 0);
}

