#include "pch.h"	
#include "ExplosionEffect.h"
#include "Framework/Graphics.h"

void ExplosionEffect::Initialize()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	m_graphics = Graphics::GetInstance();
	auto device = m_graphics->GetDeviceResources()->GetD3DDevice();

	m_vertices[0] = { Vector3(-3.0f , 3.0f,0),Vector4(1,1,1,1), Vector2(0, 0) };	//����
	m_vertices[1] = { Vector3(3.0f , 3.0f,0) ,Vector4(1,1,1,1), Vector2(1, 0) };	//�E��
	m_vertices[2] = { Vector3(-3.0f ,-3.0f,0),Vector4(1,1,1,1), Vector2(0, 1) };	//����
	m_vertices[3] = { Vector3(3.0f ,-3.0f,0) ,Vector4(1,1,1,1), Vector2(1, 1) };	//�E��

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
			L"Resources/Textures/explosion00.png",	// �摜�t�@�C���̃p�X
			nullptr,								// �����I�ȃe�N�X�`��
			m_texture.ReleaseAndGetAddressOf()		// �V�F�[�_���\�[�X�r���[(�\���p)
		)
	);
}

void ExplosionEffect::Set(DirectX::SimpleMath::Vector3 pos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;



	for (int i = 0; i < MAX_EFFECT_NUM; i++)
	{
		// �|�W�V�����Ɛ������Ԃ�ݒ�
		m_posLife.insert({ pos + Vector3(rand() % 3,rand() % 3,rand() % 3),0.0f - i * 0.1f});
	}


	m_u = 0;
	m_v = 0;
}

void ExplosionEffect::Update(float elapsedTime)
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

void ExplosionEffect::Render()
{
	using namespace DirectX::SimpleMath;

	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto states =  m_graphics->GetCommonStates();
	auto& view =    m_graphics->GetViewMatrix();
	auto& proj =    m_graphics->GetProjectionMatrix();

	for (auto& posLife : m_posLife)
	{
		if (posLife.second < 0) continue;
		// �e��ϊ�
		Matrix world = Matrix::CreateScale(1.0f * (0.5f + posLife.second));
		world *= Matrix::CreateTranslation(posLife.first + Vector3{0,posLife.second,0});

		// ���[���h�s����X�V����
		Matrix billboardMat = view.Invert();
		billboardMat._41 = 0;
		billboardMat._42 = 0;
		billboardMat._43 = 0;
		billboardMat *= world;

		m_graphics->DrawPrimitiveBegin(view, proj, billboardMat, { 1,0.25f,0,1 - posLife.second });
		m_graphics->GetPrimitiveBatch()->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[3], m_vertices[2]);
		m_graphics->DrawPrimitiveEnd();

	}


}

void ExplosionEffect::Finalize()
{
}
