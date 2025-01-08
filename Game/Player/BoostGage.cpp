#include "pch.h"
#include "BoostGage.h"
#include "Framework/Graphics.h"
#include "Framework/BinaryFile.h"

///	<summary>
///	�C���v�b�g���C�A�E�g
///	</summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> EnergyGage::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR" ,     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0},
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

EnergyGage::EnergyGage()
{
	m_boostPoint = 100;
	m_maxBoostPoint = 100;
	m_tmpPoint = 0;
	m_nowTime = 0;
}

EnergyGage::~EnergyGage()
{
}

void EnergyGage::Initialize()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	m_graphics = Graphics::GetInstance();
	auto device = m_graphics->GetDeviceResources()->GetD3DDevice();

	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(m_graphics->GetDeviceResources()->GetD3DDeviceContext());

	m_noEnergyTexture = std::make_unique<UI>(L"Resources/Textures/NoEnergy.png");
	m_noEnergyTexture->Initialize();

	//	�R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/CircleVS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/CirclePS.cso");

	DirectX::CreateWICTextureFromFile(m_graphics->GetDeviceResources()->GetD3DDevice(), L"Resources/Textures/BulletCircle.png", nullptr, m_texture.ReleaseAndGetAddressOf());

	RECT rect = m_graphics->GetDeviceResources()->GetOutputSize();
	float aspectX = float(rect.right) / float(rect.bottom);

	m_vertex[0] = { Vector3(-0.75f , -0.75f * aspectX, 0),Vector4::One, Vector2(0.0f, 0.0f) };		//����
	m_vertex[1] = { Vector3( 0.75f , -0.75f * aspectX, 0),Vector4::One, Vector2(1.0f, 0.0f) };		//�E��
	m_vertex[2] = { Vector3(-0.75f ,  0.75f * aspectX, 0),Vector4::One, Vector2(0.0f, 1.0f) };		//����
	m_vertex[3] = { Vector3( 0.75f ,  0.75f * aspectX, 0),Vector4::One, Vector2(1.0f, 1.0f) };		//�E��

	//	�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	//	���_�V�F�[�_�쐬
	device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf());

	//	�s�N�Z���V�F�[�_�쐬
	device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf());

	//	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

void EnergyGage::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	if (m_boostPoint <= 0)
	{
		m_tmpPoint = 100;
		m_vertex[0].color = Vector4(1, 0, 0, 1);		//����
		m_vertex[1].color = Vector4(1, 0, 0, 1);		//�E��
		m_vertex[2].color = Vector4(1, 0, 0, 1);		//����
		m_vertex[3].color = Vector4(1, 0, 0, 1);		//�E��
		return;
	}
	if (m_tmpPoint > 0)
	{
		m_tmpPoint-=elapsedTime * 20;
		m_nowTime += elapsedTime * 5;
		float n = sinf(m_nowTime) / 2 + 0.5f;
		m_color = { n,n,n,n };
		if (m_tmpPoint <= 0)
		{
			m_boostPoint = 100;
			m_vertex[0].color = Vector4::One;		//����
			m_vertex[1].color = Vector4::One;		//�E��
			m_vertex[2].color = Vector4::One;		//����
			m_vertex[3].color = Vector4::One;		//�E��
		}
	}
}

void EnergyGage::Render()
{
	using namespace DirectX::SimpleMath;

	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state = m_graphics->GetCommonStates();

	//	�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	if (m_tmpPoint > 0)
	{
		RECT windowSize = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

		m_noEnergyTexture->Render({ float(windowSize.right) / 2,float(windowSize.bottom) / 2 + 100 }, m_color, DirectX::SimpleMath::Vector2(936, 143) / 2, { 0.25f, 0.25f });

		cbuff.rotate = DirectX::XMConvertToRadians(90) - DirectX::XMConvertToRadians(90 * ((100 - m_tmpPoint) / 100));
	}
	else
	{
		cbuff.rotate = DirectX::XMConvertToRadians(90) - DirectX::XMConvertToRadians(90 * (m_boostPoint / 100));
	}


	//	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//	�������`��w��
	ID3D11BlendState* blendstate = state->NonPremultiplied();

	//	�������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	//	�[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(state->DepthNone(), 0);

	//	�J�����O�͍�����
	context->RSSetState(state->CullNone());


	//	�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { state->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//	�V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	//	�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	//	�|���S����`��
	m_batch->Begin();
	m_batch->DrawQuad(m_vertex[0], m_vertex[1], m_vertex[3], m_vertex[2]);
	m_batch->End();

	//	�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

float EnergyGage::GetEnergyPoint()
{
	if (m_tmpPoint > 0)
	{
		return 0;
	}
	return m_boostPoint;
}
