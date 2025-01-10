#include "pch.h"
#include "SceneMask.h"
#include "Framework/BinaryFile.h"

///	<summary>
///	�C���v�b�g���C�A�E�g
///	</summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> SceneMask::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

SceneMask::SceneMask()
	: m_isOpen{},
	  m_isClose{}
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	m_graphics = Graphics::GetInstance();

	auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	
	m_batch = std::make_unique<PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	// �摜�����[�h����
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/SceneChangeBlack.png",
			nullptr,
			m_texture.ReleaseAndGetAddressOf()
		)
	);


	m_vertex[0] = { Vector3(-1 ,  1, 0),Vector4::One, Vector2(0.0f, 0.0f) };		//����
	m_vertex[1] = { Vector3( 1 ,  1, 0),Vector4::One, Vector2(1.0f, 0.0f) };		//�E��
	m_vertex[2] = { Vector3(-1 , -1, 0),Vector4::One, Vector2(0.0f, 1.0f) };	//����
	m_vertex[3] = { Vector3( 1 , -1, 0),Vector4::One, Vector2(1.0f, 1.0f) };		//�E��

	BinaryFile vs = BinaryFile::LoadFile(L"Resources/Shaders/SceneMaskVS.cso");
	BinaryFile ps = BinaryFile::LoadFile(L"Resources/Shaders/SceneMaskPS.cso");
	device->CreateVertexShader(vs.GetData(), vs.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf());
	device->CreatePixelShader(ps.GetData(), ps.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf());

	//	�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		vs.GetData(), vs.GetSize(),
		m_inputLayout.GetAddressOf());

	//	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

}

bool SceneMask::Update(float elapsedTime)
{
	if (m_isOpen)
	{
		// �t�F�[�h�C��
		RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
		m_windowSize = float(rect.right) / float(1280);
		m_time -= elapsedTime * 2;

		if (m_time <= 0)
		{
			m_isOpen = false;
		}
	}
	else if (m_isClose)
	{
		// �t�F�[�h�A�E�g
		RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
		m_windowSize = float(rect.right) / float(1280);
		m_time += elapsedTime * 2;

		if (m_time >= 1)
		{
			m_isClose = false;
			Open();
			return true;
		}
	}
	return false;
}

void SceneMask::Render()
{
	using namespace DirectX::SimpleMath;


	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state = m_graphics->GetCommonStates();

	//	�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.time = m_time;

	//	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

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
	context->PSSetConstantBuffers(0, 1, m_CBuffer.GetAddressOf());

	//	�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	Vector4 alpha = { 1, 1, 1, m_time };
	RECT size = { 0,0,1280,720 };
	m_batch->Begin();
	m_batch->DrawQuad(m_vertex[0], m_vertex[1], m_vertex[3], m_vertex[2]);
	m_batch->End();


	//m_spriteBatch->Begin();
	//m_spriteBatch->Draw(m_texture.Get(), Vector2::Zero,&size, alpha, 0.0f, Vector2::Zero,1.0f * m_windowSize);
	//m_spriteBatch->End();
}