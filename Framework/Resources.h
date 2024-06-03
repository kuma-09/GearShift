#pragma once
#ifndef RESOURCES_DEFINED
#define RESOURCES_DEFINED

#include "Model.h"
#include "Graphics.h"

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
	// �e�N�X�`�����擾
	ID3D11ShaderResourceView* GetTexture() { return m_texture.Get(); }
public:
	Resources(Resources&&) = default;
	Resources& operator= (Resources&&) = default;
	Resources& operator= (Resources const&) = delete;
	// �f�X�g���N�^
	~Resources() = default;
	// Resouces�N���X�̃C���X�^���X���擾����
	static Resources* const GetInstance();

	void Initalize(Graphics* graphics);

	// ���\�[�X�����[�h����
	void LoadResource();

private:
	// �R���X�g���N�^
	Resources() noexcept
		:
		m_graphics{},										// �O���t�B�b�N�X
		m_device{},											// �f�o�C�X
		m_player{},											// ���f��
		m_dice{},
		m_texture{}											// �e�N�X�`��

	{
	}

private:
	// ���\�[�X
	static std::unique_ptr<Resources> m_resources;
	// �O���t�B�b�N�X
	Graphics* m_graphics;
	// �f�o�C�X
	ID3D11Device* m_device;

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

	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};

#endif		// RESOURCES_DEFINED
