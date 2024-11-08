#include "pch.h"
#include "BulletMagazine.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

BulletMagazine::BulletMagazine() :
	m_size{},
	m_pos{ 100,100 },
	m_number{0},
	digit{}
{
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/bulletMagazine.png", nullptr,
			m_bulletTexture.ReleaseAndGetAddressOf())
	);

	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/comboNumber.png", nullptr,
			m_comboTexture.ReleaseAndGetAddressOf())
	);
}

BulletMagazine::~BulletMagazine()
{
}

void BulletMagazine::SetSpriteBatch(DirectX::SpriteBatch* spriteBatch)
{
	_Unreferenced_parameter_(spriteBatch);
}


void BulletMagazine::Initialize(int number)
{


	m_pos = Vector2{1800,1000};

	digit = int(std::to_string(number).length());

	m_number = number;

}

void BulletMagazine::Update()
{
	m_pos.y -= 1;
	if (m_pos.y < 90)
	{
		m_pos.y = 90;
	}
}


void BulletMagazine::Render()
{
	auto states = Graphics::GetInstance()->GetCommonStates();

	m_size = { 0,0,500,500 };
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());
	m_spriteBatch->Draw(m_bulletTexture.Get(), Vector2(1500, 900),&m_size,Colors::White,0.0f,Vector2::Zero,0.5f);
	for (int i = 0; i < digit; i++)
	{
		int tmp = m_number % int(std::pow(10, i + 1)) / int(std::pow(10, i));
		//”Žš‚Ì‘å‚«‚³
		m_size = { 0,0,60,60 };
		m_size.left += tmp * 60;
		m_size.right += tmp * 60;
		m_spriteBatch->Draw(m_comboTexture.Get(), Vector2(m_pos.x - i * 40, m_pos.y), &m_size, Colors::White, 0.f, Vector2::Zero, 1.f);
	}
	m_spriteBatch->End();
}