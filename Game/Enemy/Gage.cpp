#include "pch.h"
#include "Gage.h"

void Enemy::Initialize()
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	m_graphics = Graphics::GetInstance();
	m_deviceResources = m_graphics->GetDeviceResources();

	m_position = Vector3(0, 0, 0);

	m_primitive = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>();

	// ���̓��C�A�E�g���쐬����
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionTexture>(
			m_deviceResources->GetD3DDevice(),
			m_graphics->GetBasicEffect(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);

	m_vertices[0] = { Vector3(-0.5f, 0.5f,0.0f),Vector4(255,255,255,255) };	// ����
	m_vertices[1] = { Vector3( 0.5f, 0.5f,0.0f),Vector4(255,255,255,255) };	// �E��
	m_vertices[2] = { Vector3(-0.5f,-0.5f,0.0f),Vector4(255,255,255,255) };	// ����
	m_vertices[3] = { Vector3( 0.5f,-0.5f,0.0f),Vector4(255,255,255,255) };	// �E��

}

void Enemy::Update()
{

}

void Enemy::Render()
{

	auto context = m_deviceResources->GetD3DDeviceContext();
	auto states = m_graphics->GetCommonStates();
	auto basicEffect = m_graphics->GetBasicEffect();

	// �|���S����`�悷�邽�߂Ɋe��p�����[�^��ݒ肷��
	context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);	// Trident���S�́uNonPremultiplied�v
	context->OMSetDepthStencilState(states->DepthDefault(), 0);					// �[�x����read��write
	context->RSSetState(states->CullCounterClockwise());						// �|���S���̗��ʂ͕`�悵�Ȃ�
	context->IASetInputLayout(m_inputLayout.Get());								// �K�؂ȓ��̓��C�A�E�g���w�肷��


	basicEffect->SetView(m_graphics->GetViewMatrix());				// �r���[�s����w�肷��
	basicEffect->SetProjection(m_graphics->GetProjectionMatrix());	// �ˉe�s����w�肷��
	basicEffect->Apply(context);				// �x�[�V�b�N�G�t�F�N�g�̕ύX��K�p����

	// �r���{�[�h��`�悷��
	m_primitive->Begin();
	m_primitive->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[3], m_vertices[2]);
	m_primitive->End();
}

void Enemy::Finalize()
{

}