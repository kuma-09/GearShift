#pragma once
#ifndef RESOURCES_DEFINED
#define RESOURCES_DEFINED

#include "Model.h"
#include "Graphics.h"
#include "Game/Shader/Shadow.h"

// Resources�N���X���`����
class Resources
{
public:
	// ���f�����擾����
	DirectX::Model* GetPlayerModel()	 { return m_player.get(); }
	DirectX::Model* GetHeadModel()		 { return m_head.get(); }
	DirectX::Model* GetBodyTopModel()	 { return m_bodyTop.get(); }
	DirectX::Model* GetBodyBottomModel() { return m_bodyBottom.get(); }
	DirectX::Model* GetlArmModel()		 { return m_lArm.get(); }
	DirectX::Model* GetrArmModel()		 { return m_rArm.get(); }
	DirectX::Model* GetlLegModel()		 { return m_lLeg.get(); }
	DirectX::Model* GetrLegModel()		 { return m_rLeg.get(); }
	DirectX::Model* GetDiceModel()		 { return m_dice.get(); }
	DirectX::Model* GetCubeModel()		 { return m_cube.get(); }
	DirectX::Model* GetSkyDome()		 { return m_skydome.get(); }
	DirectX::Model* GetFloorModel()		 { return m_floor.get(); }
	DirectX::Model* GetBillAModel()		 { return m_billA.get(); }
	DirectX::Model* GetBillBModel()		 { return m_billB.get(); }
	DirectX::Model* GetDropItemModel()	 { return m_dropItem.get(); }
	DirectX::Model* GetBulletModel()	 { return m_bullet.get(); }

	// �e�N�X�`�����擾
	ID3D11ShaderResourceView* GetGreenTexture() { return m_greenTexture.Get(); }

	Shadow* GetShadow() { return m_shadow.get(); }
public:
	Resources(Resources&&) = default;
	Resources& operator= (Resources&&) = default;
	Resources& operator= (Resources const&) = delete;
	// �f�X�g���N�^
	~Resources();
	// Resouces�N���X�̃C���X�^���X���擾����
	static Resources* const GetInstance();

	// ���\�[�X�����[�h����
	void LoadResource(Graphics* graphics);

private:
	// �R���X�g���N�^
	Resources() noexcept
	{
	}

private:
	// ���\�[�X
	static std::unique_ptr<Resources> m_resources;

	// ���f��
	std::unique_ptr<DirectX::Model> m_player;
	std::unique_ptr<DirectX::Model> m_head;
	std::unique_ptr<DirectX::Model> m_bodyTop;
	std::unique_ptr<DirectX::Model> m_bodyBottom;
	std::unique_ptr<DirectX::Model> m_lArm;
	std::unique_ptr<DirectX::Model> m_rArm;
	std::unique_ptr<DirectX::Model> m_lLeg;
	std::unique_ptr<DirectX::Model> m_rLeg;
	std::unique_ptr<DirectX::Model> m_dice;
	std::unique_ptr<DirectX::Model> m_cube;
	std::unique_ptr<DirectX::Model> m_skydome;
	std::unique_ptr<DirectX::Model> m_floor;
	std::unique_ptr<DirectX::Model> m_billA;
	std::unique_ptr<DirectX::Model> m_billB;
	std::unique_ptr<DirectX::Model> m_dropItem;
	std::unique_ptr<DirectX::Model> m_bullet;

	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_greenTexture;

	// �e
	std::unique_ptr<Shadow> m_shadow;
};

#endif		// RESOURCES_DEFINED
