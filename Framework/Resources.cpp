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
	m_player = DirectX::Model::CreateFromSDKMESH(m_device, L"Resources\\Models\\robot.sdkmesh", *m_graphics->GetFX());
	m_head = DirectX::Model::CreateFromSDKMESH(m_device, L"Resources\\Models\\head.sdkmesh", *m_graphics->GetFX());
	m_body = DirectX::Model::CreateFromSDKMESH(m_device, L"Resources\\Models\\body.sdkmesh", *m_graphics->GetFX());
	m_lArm = DirectX::Model::CreateFromSDKMESH(m_device, L"Resources\\Models\\lArm.sdkmesh", *m_graphics->GetFX());
	m_rArm = DirectX::Model::CreateFromSDKMESH(m_device, L"Resources\\Models\\rArm.sdkmesh", *m_graphics->GetFX());
	m_lLeg = DirectX::Model::CreateFromSDKMESH(m_device, L"Resources\\Models\\lLeg.sdkmesh", *m_graphics->GetFX());
	m_rLeg = DirectX::Model::CreateFromSDKMESH(m_device, L"Resources\\Models\\rLeg.sdkmesh", *m_graphics->GetFX());
	m_dice = DirectX::Model::CreateFromCMO(m_device, L"Resources\\Models\\dice.cmo", *m_graphics->GetFX());
	// テクスチャをロードする
	DirectX::CreateWICTextureFromFile(
		m_device,
		L"Resources/Textures/Daylight_Box.png",
		nullptr,
		m_texture.GetAddressOf()
	);
}
