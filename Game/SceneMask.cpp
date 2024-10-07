#include "pch.h"
#include "SceneMask.h"

SceneMask::SceneMask()
	: m_isOpen{},
	  m_isClose{}
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	m_graphics = Graphics::GetInstance();

	auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	// 画像をロードする
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/SceneChangeBlack.png",
			nullptr,
			m_texture.ReleaseAndGetAddressOf()
		)
	);
}

bool SceneMask::Update(float elapsedTime)
{
	if (m_isOpen)
	{
		// フェードイン

		m_alpha -= elapsedTime * 2;

		if (m_alpha <= 0)
		{
			m_isOpen = false;
		}
	}
	else if (m_isClose)
	{
		// フェードアウト

		m_alpha += elapsedTime * 2;

		if (m_alpha >= 1)
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

	Vector4 alpha = { 1, 1, 1, m_alpha };

	m_spriteBatch->Begin();
	m_spriteBatch->Draw(m_texture.Get(), Vector2::Zero,alpha);
	m_spriteBatch->End();
}