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

	DirectX::EffectFactory* FX = new DirectX::EffectFactory(device);


	// リソースディレクトリを設定する
	FX->SetDirectory(L"Resources\\Models");
	// モデルをロードする
	m_player	 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\old_robot\\Robot.sdkmesh",       *graphics->GetFX());
	m_head		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\old_robot\\Head.sdkmesh",        *graphics->GetFX());
	m_bodyTop	 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\old_robot\\Body.sdkmesh",     *graphics->GetFX());
	m_lArm		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\old_robot\\LeftArm.sdkmesh",        *graphics->GetFX());
	m_rArm		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\old_robot\\RightArm.sdkmesh",        *graphics->GetFX());
	m_lLeg		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\old_robot\\LeftLeg.sdkmesh",        *graphics->GetFX());
	m_rLeg		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\old_robot\\RightLeg.sdkmesh",        *graphics->GetFX());
	m_dice		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\brain-robot.sdkmesh",            *graphics->GetFX());
	m_cube		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\cube.sdkmesh",        *graphics->GetFX());
	m_skydome    = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\skydome.sdkmesh",     *graphics->GetFX());
	m_floor      = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\floor-small-square.sdkmesh", *graphics->GetFX());
	m_billA      = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\large_buildingA.sdkmesh", *graphics->GetFX());
	m_billB		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\large_buildingB.sdkmesh", *graphics->GetFX());
	m_dropItem   = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\mese.sdkmesh", *graphics->GetFX());
	m_boom       = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\boom.sdkmesh", *graphics->GetFX());
	m_tankBody   = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\TankBody.sdkmesh", *graphics->GetFX());
	m_entyu      = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\entyu.sdkmesh", *graphics->GetFX());

	// テクスチャをロードする
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/green.png",
		nullptr,
		m_greenTexture.GetAddressOf()
	);

	delete FX;
	FX = nullptr;

	// 影
	m_shadow = std::make_unique<Shadow>();
	m_shadow->Initialize();
}
