#include "pch.h"
#include "StartAnimation.h"
#include "Framework/Graphics.h"
#include "Framework/Easing.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

StartAnimation::StartAnimation()
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
			L"Resources/Textures/bar.png",
			nullptr,
			m_texture.ReleaseAndGetAddressOf()
		)
	);


	m_nowTime = 0;
	m_result = 0;
	m_targetTime = 1.0f;
}

bool StartAnimation::Update(float elapsedTime)
{
	m_nowTime += elapsedTime;
	if (m_nowTime <= m_targetTime)
	{
		m_result = Easing::InQuart(m_nowTime, m_targetTime);
		return false;
	}
	if (m_nowTime <= m_targetTime * 2)
	{
		m_result = Easing::InQuart(m_nowTime - m_targetTime, m_targetTime);
		m_result += 1.f;
		return false;
	}
	return false;
}


void StartAnimation::Render()
{
	auto states = Graphics::GetInstance()->GetCommonStates();

	// �X�v���C�g�o�b�`�̊J�n�F�I�v�V�����Ń\�[�g���[�h�A�u�����h�X�e�[�g���w�肷��
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());;
	// �F(���ߓx)�����߂�
	RECT windowsize = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
	int x, y;
	Graphics::GetInstance()->GetScreenSize(x,y);
	float value = float(windowsize.right) / x;

	Vector4 color{ 1.0f,1.0f,1.0f, 0.3f };
	m_spriteBatch->Draw(m_texture.Get(),Vector2{ ( - 1280 + 1280 * m_result) * value, float(windowsize.bottom) / 2.f}, 0, color, 0, Vector2::Zero, Vector2{1 * value, 1 * value});
	m_spriteFont->DrawString(m_spriteBatch.get(), L"Mission Start", Vector2{ ( - 1280 + 1280 * m_result) * value, float(windowsize.bottom) / 2.f}, DirectX::Colors::Black,0,Vector2::Zero,Vector2{ 1 * value, 1 * value });

	// �X�v���C�g�o�b�`�̏I���
	m_spriteBatch->End();
}