#include "pch.h"
#include "ExBulletMagazine.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

ExBulletMagazine::ExBulletMagazine() :
	m_size{},
	m_pos{ 100,100 },
	m_alpha{0},
	m_digit{}
{
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/ExBulletMagazine.png", nullptr,
			m_bulletTexture.ReleaseAndGetAddressOf())
	);

	m_number = std::make_unique<Number>();
}

ExBulletMagazine::~ExBulletMagazine()
{
}

void ExBulletMagazine::SetSpriteBatch(DirectX::SpriteBatch* spriteBatch)
{
	UNREFERENCED_PARAMETER(spriteBatch);
}


void ExBulletMagazine::Initialize(int number)
{
	m_pos = Vector2{1200,550};
	m_digit = int(std::to_string(number).length());
	m_number->Initialize({ 1200,550 });
	m_number->SetNum(number);
}

void ExBulletMagazine::Update(float elapsedTime, int number)
{
	//m_pos.y -= 1;
	//if (m_pos.y < 540)
	//{
	//	m_pos.y = 540;
	//}
	m_alpha += elapsedTime * 5;
	m_digit = int(std::to_string(number).length());
	m_number->SetNum(number);
}


void ExBulletMagazine::Render(bool isActive)
{
	auto states = Graphics::GetInstance()->GetCommonStates();

	RECT windowsize = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
	int x, y;
	Graphics::GetInstance()->GetScreenSize(x, y);
	float value = float(windowsize.right) / x;
	DirectX::XMVECTORF32 color = { 1,1,1,0.5f };
	if (isActive) color = { 1,1,1,sinf(m_alpha) * 0.5f + 0.5f };


	m_size = { 0,0,500,500 };
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());
	m_spriteBatch->Draw(m_bulletTexture.Get(), Vector2(1000, 500) * value, &m_size, color, 0.0f, Vector2::Zero, 0.3f * value);
	m_spriteBatch->End();
	m_number->Render();
}