#include "pch.h"
#include "StartAnimation.h"
#include "Framework/Graphics.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

StartAnimation::StartAnimation():
	m_size{},
	m_count{},
	m_height{},
	m_isChangeScene{},
	m_posX{},
	m_posY{},
	m_postAlpha{},
	m_postPosX{},
	m_startScale{},
	m_targetNumber{},
	m_width{}
{
}

StartAnimation::~StartAnimation()
{
}

void StartAnimation::Initialize()
{
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();


	// �X�v���C�g�o�b�`���쐬����
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	// �X�v���C�g�t�H���g���쐬����
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(
		device,
		L"Resources/Fonts/BigGothic.spritefont"
	);

	// �摜�����[�h����
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/barx1920.png",
			nullptr,
			m_texture.ReleaseAndGetAddressOf()
		)
	);
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/GO.png",
			nullptr,
			m_GoTexture.ReleaseAndGetAddressOf()
		)
	);

	m_width = 1;
	m_height = 0.001f;
	m_posX = 4;
	m_posY = 200;
	m_count = 0;
	m_startScale = 0;
	m_startScale = 0.001f;

	m_postPosX = 2;
	m_postAlpha = 0.001f;


	// �V�[���ύX�t���O������������
	m_isChangeScene = false;


}

bool StartAnimation::Update(float elapsedTime)
{
	// �^�񒆂̃o�[
	m_height *= 1.2f;
	if (m_height >= 1.f)
	{
		m_height = 1.f;
		m_posX *= 1.2f;
	}

	// MissionStart�̕���
	if (m_posX >= 1000)
	{
		m_posX = 1000;
		m_count += elapsedTime;
	}

	if (m_count >= 1)
	{

		m_startScale *= 1.3f;
		if (m_startScale >= 1.f)
		{
			m_startScale = 1.f;
			m_postPosX *= 1.2f;
			m_postAlpha *= 1.2f;
			if (m_postPosX >= 890)
			{
				m_postAlpha = 1;
				return false;
			}
		}
	}
	return true;
}


void StartAnimation::Render()
{
	auto states = Graphics::GetInstance()->GetCommonStates();

	// �X�v���C�g�o�b�`�̊J�n�F�I�v�V�����Ń\�[�g���[�h�A�u�����h�X�e�[�g���w�肷��
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());

	// �F(���ߓx)�����߂�
	Vector4 color{ 1.0f,1.0f,1.0f, 0.3f };

	m_spriteBatch->Draw(m_texture.Get(), Vector2(0, 360 + m_posY), nullptr, Vector4(1.0f, 1.0f, 1.0f, 0.3f - m_startScale), 0.f, Vector2(0, 120), Vector3(m_width, m_height, 0));

	m_spriteFont->DrawString(m_spriteBatch.get(), L"Mission Start", Vector2(m_posX - 401, 321 + m_posY), Vector4(0.0f, 0.0f, 0.0f, 1.0f - m_startScale), 0, Vector2(0, 0), 1.f);
	m_spriteFont->DrawString(m_spriteBatch.get(), L"Mission Start", Vector2(m_posX - 398, 321 + m_posY), Vector4(0.0f, 0.0f, 0.0f, 1.0f - m_startScale), 0, Vector2(0, 0), 1.f);
	m_spriteFont->DrawString(m_spriteBatch.get(), L"Mission Start", Vector2(m_posX - 400, 320 + m_posY), Vector4(1.0f, 1.0f, 1.0f, 1.0f - m_startScale), 0, Vector2(0, 0), 1.f);
	m_spriteFont->DrawString(m_spriteBatch.get(), L"Mission Start", Vector2(m_posX - 399, 321 + m_posY), Vector4(0.0f, 0.0f, 0.0f, 1.0f - m_startScale), 0, Vector2(0, 0), 1.f);

	// �X�v���C�g�o�b�`�̏I���
	m_spriteBatch->End();
}