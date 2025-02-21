#include <pch.h>
#include "OutLine.h"
#include "Framework/BinaryFile.h"

/// <summary>
/// �C���v�b�g���C�A�E�g
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> PostOutLine::INPUT_LAYOUT =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

PostOutLine::PostOutLine()
{
}

PostOutLine::~PostOutLine()
{
}

void PostOutLine::Initialize()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
    auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

    RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

    // �����_�[�e�N�X�`���̍쐬�i�V�[���S�́j
    m_offscreenRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
    m_offscreenRT->SetDevice(device);
    m_offscreenRT->SetWindow(rect);

    LoadShader(device);

    // �v���~�e�B�u�o�b�`�̍쐬
    m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(context);

    m_vertex[0] = { Vector3(-1.0f , 1.0f,0), Vector2(0, 0) };	//����
    m_vertex[1] = { Vector3(1.0f , 1.0f,0), Vector2(1, 0) };	//�E��
    m_vertex[2] = { Vector3(-1.0f ,-1.0f,0), Vector2(0, 1) };	//����
    m_vertex[3] = { Vector3(1.0f ,-1.0f,0), Vector2(1, 1) };	//�E��
}

void PostOutLine::Update(float elapsedTime)
{
    UNREFERENCED_PARAMETER(elapsedTime);
    RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
    m_offscreenRT->SetWindow(rect);
}

void PostOutLine::Render(ID3D11ShaderResourceView* depthTexture, ID3D11ShaderResourceView* normalTexture)
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
    ID3D11ShaderResourceView* textures[] = {depthTexture,normalTexture};

	context->IASetInputLayout(m_inputLayout.Get());
	context->VSSetShader(m_VS.Get(), nullptr, 0);
	context->PSSetShader(m_PS.Get(), nullptr, 0);
    context->PSSetShaderResources(1, 2, textures);

    m_batch->Begin();
    m_batch->DrawQuad(m_vertex[0], m_vertex[1], m_vertex[3], m_vertex[2]);
    m_batch->End();
}

void PostOutLine::LoadShader(ID3D11Device* device)
{

	// ���C�g�p�̃s�N�Z���V�F�[�_�����[�h����
	BinaryFile ps = BinaryFile::LoadFile(L"Resources/Shaders/PostOutLinePS.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(ps.GetData(), ps.GetSize(), nullptr, m_PS.ReleaseAndGetAddressOf())
	);

	// ���C�g�p�̃s�N�Z���V�F�[�_�����[�h����
	BinaryFile vs = BinaryFile::LoadFile(L"Resources/Shaders/PostOutLineVS.cso");
	DX::ThrowIfFailed(
		device->CreateVertexShader(vs.GetData(), vs.GetSize(), nullptr, m_VS.ReleaseAndGetAddressOf())
	);

	//	�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		vs.GetData(), vs.GetSize(),
		m_inputLayout.GetAddressOf()
	);

}
