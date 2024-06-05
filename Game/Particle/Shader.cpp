//--------------------------------------------------------------------------------------
// File: Shader.h
//
// �p�[�e�B�N���N���X
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Shader.h"
#include "Framework/Graphics.h"
#include "Framework/BinaryFile.h"

using namespace DirectX;

/// <summary>
/// �C���v�b�g���C�A�E�g
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> Shader::INPUT_LAYOUT =
{
	{ "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

Microsoft::WRL::ComPtr<ID3D11Buffer> Shader::m_CBuffer;

/// <summary>
/// �R���X�g���N�^
/// </summary>
Shader::Shader()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Shader::~Shader()
{
}

/// <summary>
/// Shader�쐬�����������������֐�
/// </summary>
void Shader::CreateShader()
{
	auto device  = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = Graphics::GetInstance()->GetCommonStates();
	//	�R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/VS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/PS.cso");

	//	�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	//	���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	//	�s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	//	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//	���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	////	�������`��w��
	//ID3D11BlendState* blendstate = states->NonPremultiplied();

	////	�������菈��
	//context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	////	�[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	//context->OMSetDepthStencilState(states->DepthDefault(), 0);

	////	�J�����O�͂Ȃ�
	//context->RSSetState(states->CullNone());

	//	�V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);


	//	�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());
}


void Shader::Render()
{



		//	�|���S����`��
		m_batch->Begin();
		//m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
		m_batch->End();

		////	�V�F�[�_�̓o�^���������Ă���
		//context->VSSetShader(nullptr, nullptr, 0);
		//context->PSSetShader(nullptr, nullptr, 0);
}


// Update�ōX�V����ׂ����
////	�r���[�ݒ�
//cbuff.matView = view.Transpose();
////	�v���W�F�N�V�����ݒ�
//cbuff.matProj = proj.Transpose();
////	���[���h�ݒ�
//cbuff.matWorld = m_world.Transpose();
//cbuff.Diffuse = SimpleMath::Vector4(1, 1, 1, 1);
//
////	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
//context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);