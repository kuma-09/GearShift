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

// ���\�[�X�����[�h����
void Resources::LoadResource(Graphics* graphics)
{
	auto device = graphics->GetDeviceResources()->GetD3DDevice();

	// ���\�[�X�f�B���N�g����ݒ肷��
	graphics->GetFX()->SetDirectory(L"Resources\\Models");
	// ���f�������[�h����
	m_player	 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\robot.sdkmesh",       *graphics->GetFX());
	m_head		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\head.sdkmesh",        *graphics->GetFX());
	m_bodyTop	 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\bodyTop.sdkmesh",     *graphics->GetFX());
	m_bodyBottom = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\bodyBottom.sdkmesh",  *graphics->GetFX());
	m_lArm		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\lArm.sdkmesh",        *graphics->GetFX());
	m_rArm		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\rArm.sdkmesh",        *graphics->GetFX());
	m_lLeg		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\lLeg.sdkmesh",        *graphics->GetFX());
	m_rLeg		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\rLeg.sdkmesh",        *graphics->GetFX());
	m_dice		 = DirectX::Model::CreateFromCMO	(device, L"Resources\\Models\\dice.cmo",            *graphics->GetFX());
	m_cube		 = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\cube.sdkmesh",        *graphics->GetFX());
	m_skydome    = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\skydome.sdkmesh",     *graphics->GetFX());
	m_floor      = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\floor-small-square.sdkmesh", *graphics->GetFX());
	m_billA      = DirectX::Model::CreateFromSDKMESH(device, L"Resources\\Models\\large_buildingA.sdkmesh", *graphics->GetFX());

	// �e�N�X�`�������[�h����
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/green.png",
		nullptr,
		m_greenTexture.GetAddressOf()
	);
}
