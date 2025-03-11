#pragma once

#include "Model.h"
#include "Graphics.h"
#include <unordered_map>

/// <summary>
/// ���f���f�[�^���Ǘ�����N���X
/// �V���O���g��
/// </summary>
class Resources
{
public:

	// ���f���̎��
	enum ModelType
	{
		Player,
		Head,
		Body,
		LArm,
		RArm,
		LLeg,
		RLeg,


		HomingEnemy,
		TankBody,
		BossLeg,
		BossHead,

		Cube,
		SkyDome,
		Floor,
		Floor2,
		BillA,
		BillB,
		BillC,

		Cloud1,
		Cloud2,
		Cloud3,

		DropItem,

		Boom,

		Gun,
		MissileLuncher,

		Light,
	};

	// ���f�����擾����
	DirectX::Model* GetModel(ModelType type);

	// �e�N�X�`�����擾
	ID3D11ShaderResourceView* GetWhiteTexture() { return m_Texture.Get(); }

public:
	Resources(Resources&&) = default;
	Resources& operator= (Resources&&) = default;
	Resources& operator= (Resources const&) = delete;
	// �f�X�g���N�^
	~Resources();
	// Resouces�N���X�̃C���X�^���X���擾����
	static Resources* const GetInstance();

	// ���\�[�X��ǂݍ���
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
	std::unordered_map<ModelType, std::unique_ptr<DirectX::Model>> m_umModel;

	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Texture;

};
