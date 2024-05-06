#include "pch.h"
#include "Framework/Resources.h"


std::unique_ptr<Resources> Resources::m_resources = nullptr;

Resources* const Resources::GetInstance()
{
	if (m_resources == nullptr)
	{
		m_resources.reset(new Resources());
	}
	return m_resources.get();
}

void Resources::Initalize(Graphics* graphics)
{
	m_graphics = graphics;
	m_device = m_graphics->GetDeviceResources()->GetD3DDevice();

}

// リソースをロードする
void Resources::LoadResource()
{
	// リソースディレクトリを設定する
	m_graphics->GetFX()->SetDirectory(L"Resources\\Models");
	// モデルをロードする
	m_model = DirectX::Model::CreateFromCMO(m_device, L"Resources\\Models\\dice.cmo", *m_graphics->GetFX());
	// テクスチャをロードする
	DirectX::CreateWICTextureFromFile(
		m_device,
		L"Resources/Textures/Daylight_Box.png",
		nullptr,
		m_texture.GetAddressOf()
	);
}
