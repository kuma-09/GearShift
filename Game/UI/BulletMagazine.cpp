#include "pch.h"
#include "BulletMagazine.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

BulletMagazine::BulletMagazine() :
	m_size{},
	m_pos{ 100,100 },
	m_alpha{0}
{
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/bulletMagazine.png", nullptr,
			m_bulletTexture.ReleaseAndGetAddressOf())
	);

	m_number = std::make_unique<Number>();
	m_number->Initialize({ 1200,650 });
	m_number->SetNum(0);

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
	m_pos = Vector2{1200,650};
	m_number->SetNum(number);
}

void BulletMagazine::Update(float elapsedTime, int number)
{
	m_alpha += elapsedTime * 5;
	m_number->SetNum(number);
}


void BulletMagazine::Render(bool isActive)
{
	auto states = Graphics::GetInstance()->GetCommonStates();

	RECT windowsize = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
	int x, y;
	Graphics::GetInstance()->GetScreenSize(x, y);
	float value = float(windowsize.right) / x;
	DirectX::XMVECTORF32 color = { 1,1,1,0.5f };
	if (isActive) color = { 1,1,1,sinf(m_alpha) * 0.5f + 0.5f };

	m_size = { 0,0,500,500 };
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied(), states->PointWrap());
	m_spriteBatch->Draw(m_bulletTexture.Get(), Vector2(1000, 600) * value,&m_size,color, 0.0f, Vector2::Zero, 0.3f * value);
	m_spriteBatch->End();
	m_number->Render();
}