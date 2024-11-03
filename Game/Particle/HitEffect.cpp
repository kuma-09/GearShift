#include "pch.h"	
#include "HitEffect.h"
#include "Framework/Graphics.h"

void HitEffect::Initialize()
{
	using namespace DirectX;

	m_graphics = Graphics::GetInstance();
	auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(m_graphics->GetDeviceResources()->GetD3DDeviceContext());
	// �x�[�V�b�N�G�t�F�N�g���쐬����
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
	m_basicEffect->SetLightingEnabled(false);	// ���C�e�B���O(OFF)
	m_basicEffect->SetVertexColorEnabled(false);// ���_�J���[(OFF)
	m_basicEffect->SetTextureEnabled(true);		// �e�N�X�`��(ON)

	// ���̓��C�A�E�g���쐬����
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionTexture>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);


	// �摜�����[�h����
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,									// �f�o�C�X�R���e�L�X�g
			L"Resources/Textures/smoke_white_big.png",	// �摜�t�@�C���̃p�X
			nullptr,								// �����I�ȃe�N�X�`��
			m_texture.ReleaseAndGetAddressOf()		// �V�F�[�_���\�[�X�r���[(�\���p)
		)
	);
}

void HitEffect::Set(DirectX::SimpleMath::Vector3 pos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;



	for (int i = 0; i < 3; i++)
	{
		m_posLife.insert({ pos,0.0f });
	}


	m_u = 0;
	m_v = 0;
}

void HitEffect::Update(float elapsedTime)
{

	for (auto it = m_posLife.begin(); it != m_posLife.end();)
	{
		it->second += elapsedTime;
		if (it->second >= MAX_LIFETIME)
		{
			it = m_posLife.erase(it);
		}
		else
		{
			it++;
		}
	}

}

void HitEffect::Render()
{
	using namespace DirectX::SimpleMath;

	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = Graphics::GetInstance()->GetCommonStates();
	auto view = Graphics::GetInstance()->GetViewMatrix();
	auto proj = Graphics::GetInstance()->GetProjectionMatrix();

	// �e��p�����[�^���X�V����
	context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);// �u�����h�X�e�[�g
	ID3D11SamplerState* sampler = states->PointWrap();	// or PointWrap()
	context->PSSetSamplers(0, 1, &sampler);
	context->OMSetDepthStencilState(states->DepthRead(), 0);		// �[�x�o�b�t�@/�X�e���V���o�b�t�@
	context->RSSetState(states->CullCounterClockwise());						// �J�����O
	context->IASetInputLayout(m_inputLayout.Get());					// ���̓��C�A�E�g




	for (auto& posLife : m_posLife)
	{
		// �e��ϊ�
		Matrix world = Matrix::CreateScale(1.0f * (0.1f + posLife.second));
		world *= Matrix::CreateTranslation(posLife.first + Vector3{0,posLife.second * 5,0});

		// ���[���h�s����X�V����
		Matrix billboardMat = view.Invert();
		billboardMat._41 = 0;
		billboardMat._42 = 0;
		billboardMat._43 = 0;
		billboardMat *= world;


		// �G�t�F�N�g��ύX���遨���W�n��ݒ肷��
		m_basicEffect->SetWorld(billboardMat);				// ���[���h�s��
		m_basicEffect->SetView(view);				// �r���[�s��
		m_basicEffect->SetProjection(proj);	// �ˉe�s��
		m_basicEffect->SetTexture(m_texture.Get());	// �e�N�X�`��
		m_basicEffect->SetColorAndAlpha({1, 0.25f,0, 1 - posLife.second});
		m_basicEffect->Apply(context);				// �x�[�V�b�N�G�t�F�N�g���X�V����

		//int n = int(posLife.second);
		//int y = n / 3;
		//int x = n % 3;

		//// UV���W�w��p�̔z��
		//constexpr float u[4]{ 0.0f, 0.34f, 0.67f, 1.0f };
		//constexpr float v[4]{ 0.0f, 0.34f, 0.67f, 1.0f };

		//m_vertices[0] = { Vector3(-3.0f , 3.0f,0), Vector2(u[x],     v[y]) };	//����
		//m_vertices[1] = { Vector3( 3.0f , 3.0f,0),  Vector2(u[x + 1], v[y]) };	//�E��
		//m_vertices[2] = { Vector3(-3.0f ,-3.0f,0), Vector2(u[x],     v[y + 1]) };	//����
		//m_vertices[3] = { Vector3( 3.0f ,-3.0f,0),  Vector2(u[x + 1], v[y + 1]) };	//�E��

		m_vertices[0] = { Vector3(-3.0f , 3.0f,0), Vector2( 0, 0) };	//����
		m_vertices[1] = { Vector3( 3.0f , 3.0f,0), Vector2( 1, 0) };	//�E��
		m_vertices[2] = { Vector3(-3.0f ,-3.0f,0), Vector2( 0, 1) };	//����
		m_vertices[3] = { Vector3( 3.0f ,-3.0f,0), Vector2( 1, 1) };	//�E��

		m_batch->Begin();
		m_batch->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[3], m_vertices[2]);
		m_batch->End();
	}


}

void HitEffect::Finalize()
{
}
