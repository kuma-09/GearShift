#include "pch.h"
#include "FinishAnimation.h"
#include "Framework/Graphics.h"
#include "Framework/Easing.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

FinishAnimation::FinishAnimation()
{
	m_bar = std::make_unique<UI>(L"Resources/Textures/bar.png");
	m_font = std::make_unique<UI>(L"Resources/Textures/StageClear.png");
	m_nowTime = 0;
	m_result = 0;
	m_targetTime = 1.0f;
	m_isChangeScene = false;
}

FinishAnimation::~FinishAnimation()
{
}

void FinishAnimation::Initialize()
{
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();


	// �X�v���C�g�o�b�`���쐬����
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	// �摜�����[�h����
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/bar.png",
			nullptr,
			m_texture.ReleaseAndGetAddressOf()
		)
	);

	m_bar->Initialize();
	m_font->Initialize();

	m_isChangeScene = true;	
}

bool FinishAnimation::Update(float elapsedTime)
{
	if (!m_isChangeScene) return false;
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
		return true;
	}
	return false;
}


void FinishAnimation::Render()
{
	if (!m_isChangeScene) return;

	auto state = Graphics::GetInstance()->GetCommonStates();

	// �X�v���C�g�o�b�`�̊J�n�F�I�v�V�����Ń\�[�g���[�h�A�u�����h�X�e�[�g���w�肷��
	m_spriteBatch->Begin(SpriteSortMode_Deferred, state->NonPremultiplied());;

	// �F(���ߓx)�����߂�
	RECT windowsize = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
	int x, y;
	Graphics::GetInstance()->GetScreenSize(x, y);
	float value = float(windowsize.right) / x;

	XMVECTORF32 color{ 1.0f,1.0f,1.0f, 0.3f };
	m_spriteBatch->Draw(m_texture.Get(), Vector2{ (-640 + 1280 * m_result) * value, float(windowsize.bottom) / 2.f }, 0, color, 0, Vector2(1280, 240) / 2, Vector2{ 1 * value, 1 * value });

	//m_bar->Render(Vector2{ (-1280 + 1280 * m_result) * value, float(windowsize.bottom) / 2.f },color,DirectX::SimpleMath::Vector2::Zero, Vector2{ 1 * value, 1 * value });
	m_font->Render(Vector2{ (-640 + 1280 * m_result) * value, float(windowsize.bottom) / 2.f }, DirectX::Colors::White, Vector2(1200, 100) / 2, Vector2{ 1 * value, 1 * value });

	// �X�v���C�g�o�b�`�̏I���
	m_spriteBatch->End();
}