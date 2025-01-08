#include "pch.h"
#include "Number.h"
#include "Framework/Graphics.h"

Number::Number():
	m_size{},
	m_pos{ 100,100 },
	m_number{ 0 },
	m_alpha{ 0 },
	m_digit{}
{
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/0~c.png", nullptr,
			m_numberTexture.ReleaseAndGetAddressOf())
	);
}

Number::~Number()
{
}

void Number::Initialize(DirectX::SimpleMath::Vector2 pos)
{
	m_pos = pos;
}

void Number::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	auto states = Graphics::GetInstance()->GetCommonStates();

	RECT windowsize = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
	int x, y;
	Graphics::GetInstance()->GetScreenSize(x, y);
	float value = float(windowsize.right) / x;
	DirectX::XMVECTORF32 color = { 1,1,1,0.5f };


	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied(), states->PointWrap());
	for (int i = 0; i < m_digit; i++)
	{
		//”Žš‚Ì‘å‚«‚³
		m_size = { 0,0,10,10 };
		int tmp = m_number % int(std::pow(10, i + 1)) / int(std::pow(10, i));
		m_size.left += tmp * 10;
		m_size.right += tmp * 10;
		m_spriteBatch->Draw(m_numberTexture.Get(), Vector2(m_pos.x - i * m_space, m_pos.y) * value, &m_size, Colors::CornflowerBlue, 0.f, Vector2::Zero, 5.f * value);
	}
	m_spriteBatch->End();
}

void Number::RenderTime()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	auto states = Graphics::GetInstance()->GetCommonStates();
	RECT windowsize = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
	int x, y;
	Graphics::GetInstance()->GetScreenSize(x, y);
	float value = float(windowsize.right) / x;
	DirectX::XMVECTORF32 color = { 1,1,1,0.5f };

	int minutes = m_number / 60;
	int seconds = m_number - minutes * 60;

	//”Žš‚Ì‘å‚«‚³
	m_size = { 0,0,10,10 };
	Vector2 center = { 5,5 };

	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied(), states->PointWrap());
	for (int i = 0; i < 2; i++)
	{
		//”Žš‚Ì‘å‚«‚³
		m_size = { 0,0,10,10 };
		int tmp = minutes % int(std::pow(10, i + 1)) / int(std::pow(10, i));
		m_size.left += tmp * 10;
		m_size.right += tmp * 10;
		m_spriteBatch->Draw(m_numberTexture.Get(),
			Vector2(m_pos.x - i * m_space - m_space, m_pos.y) * value,
			&m_size, Colors::CornflowerBlue,
			0.f, center, 5.f * value);
	}
	for (int i = 0; i < 2; i++)
	{
		//”Žš‚Ì‘å‚«‚³
		m_size = { 0,0,10,10 };
		int tmp = seconds % int(std::pow(10, i + 1)) / int(std::pow(10, i));
		m_size.left += tmp * 10;
		m_size.right += tmp * 10;
		m_spriteBatch->Draw(m_numberTexture.Get(),
			Vector2(m_pos.x - i * m_space + m_space * 2, m_pos.y) * value,
			&m_size, Colors::CornflowerBlue,
			0.f, center, 5.f * value);
	}
	//”Žš‚Ì‘å‚«‚³
	m_size = { 0,0,10,10 };
	m_size.left = 100;
	m_size.right = 110;
	m_spriteBatch->Draw(m_numberTexture.Get(),
		Vector2(m_pos.x, m_pos.y) * value,
		&m_size, Colors::CornflowerBlue,
		0.f, center, 5.f * value);
	m_spriteBatch->End();
}

void Number::SetNum(int num)
{
	m_digit = int(std::to_string(num).length());
	m_number = num;
}