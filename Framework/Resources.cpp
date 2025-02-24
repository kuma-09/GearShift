#include "pch.h"
#include "Framework/Resources.h"


std::unique_ptr<Resources> Resources::m_resources = nullptr;

DirectX::Model* Resources::GetModel(ModelType type)
{
	auto it = m_umModel.find(type);
	if (it != m_umModel.end())
	{
		return it->second.get();
	}
	return nullptr;
}

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
	m_umModel[ModelType::Player]	     = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\old_robot\\Robot.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::Head]		     = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\old_robot\\Head.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::Body]		     = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\old_robot\\Body.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::LArm]		     = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\old_robot\\LeftArm.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::RArm]		     = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\old_robot\\RightArm.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::LLeg]		     = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\old_robot\\LeftLeg.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::RLeg]		     = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\old_robot\\RightLeg.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::HomingEnemy]    = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\brain-robot.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::TankBody]		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\TankBody.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::BossLeg]		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\BulldogLeg.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::BossHead]		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\BulldogHead.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::Cube]		     = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\cube.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::SkyDome]	     = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\skydome.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::Floor]		     = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\testFloor.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::Floor2]	     = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\testFloor2.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::BillA]		     = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\large_buildingA.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::BillB]		     = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\large_buildingB.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::BillC]		     = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\large_buildingC.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::DropItem]	     = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\mese.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::Boom]		     = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\boom.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::Gun]			 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\gun.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::MissileLuncher] = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\MissileLuncher.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::Light]			 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\light.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::Cloud1]		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\Cloud1.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::Cloud2]		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\Cloud2.sdkmesh", *graphics->GetFX());
	m_umModel[ModelType::Cloud3]		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\Cloud3.sdkmesh", *graphics->GetFX());

	// テクスチャをロードする
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/white.png",
		nullptr,
		m_Texture.GetAddressOf()
	);

	delete FX;
	FX = nullptr;

}
