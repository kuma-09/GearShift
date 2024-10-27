#include "pch.h"
#include "Framework/Resources.h"


std::unique_ptr<Resources> Resources::m_resources = nullptr;

Resources::~Resources()
{

}

Resources* const Resources::GetInstance()
{
	if (m_resources == nullptr)
	{
		m_resources.reset(new Resources());
	}
	return m_resources.get();
}

// リソースをロードする
void Resources::LoadResource(Graphics* graphics)
{
	auto device = graphics->GetDeviceResources()->GetD3DDevice();

	// リソースディレクトリを設定する
	graphics->GetFX()->SetDirectory(L"Resources\\Models");
	// モデルをロードする
	m_player	 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\Robot.sdkmesh",       *graphics->GetFX());
	m_head		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\Head.sdkmesh",        *graphics->GetFX());
	m_bodyTop	 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\Body.sdkmesh",     *graphics->GetFX());
	//m_bodyBottom = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\bodyBottom.sdkmesh",  *graphics->GetFX());
	m_lArm		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\LeftArm.sdkmesh",        *graphics->GetFX());
	m_rArm		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\RightArm.sdkmesh",        *graphics->GetFX());
	m_lLeg		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\LeftLeg.sdkmesh",        *graphics->GetFX());
	m_rLeg		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\RightLeg.sdkmesh",        *graphics->GetFX());
	m_dice		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\brain-robot.sdkmesh",            *graphics->GetFX());
	m_cube		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\cube.sdkmesh",        *graphics->GetFX());
	m_skydome    = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\skydome.sdkmesh",     *graphics->GetFX());
	m_floor      = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\floor-small-square.sdkmesh", *graphics->GetFX());
	m_billA      = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\large_buildingA.sdkmesh", *graphics->GetFX());
	m_billB		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\large_buildingB.sdkmesh", *graphics->GetFX());
	m_dropItem   = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\mese.sdkmesh", *graphics->GetFX());

	// テクスチャをロードする
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/green.png",
		nullptr,
		m_greenTexture.GetAddressOf()
	);

	// 影
	m_shadow = std::make_unique<Shadow>();
	m_shadow->Initialize();
}
